#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <cassert>

#include "game.hpp"
#include "i_performance_manager.hpp"
#include "standard_log_manager.hpp"
#include "system_variables.hpp"
#include "date_time.hpp"
#include "../win/windows_utility.hpp"
#include "log_utility.hpp"

/*ログの書式
[hh:mm:ss,frame]level:detail@place
*/

namespace {
	constexpr char *message_header(const_cast<char *>("M"));
	constexpr char *warning_header(const_cast<char *>("W"));
	constexpr char *error_header(const_cast<char *>("E"));
	constexpr char *fatal_error_header(const_cast<char *>("F"));
	constexpr size_t default_log_history_max_size(100);

	void open_console_window() {
		AllocConsole();
		FILE *fp;
		// ReSharper disable once StringLiteralTypo
		if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0) { throw std::runtime_error("コンソールウインドウを開けませんでした。"); }
		// ReSharper disable once StringLiteralTypo
		if (freopen_s(&fp, "CONIN$", "r", stdin) != 0) { throw std::runtime_error("コンソールウインドウを開けませんでした。"); }
	}

	void close_console_window() { FreeConsole(); }

	std::string add_new_line_if_need(const std::string &str) {
		std::string out = str;
		if (!str.empty() && str[str.size() - 1] != '\n') { out += '\n'; }
		return out;
	}
}

namespace plnt::private_ {
	//////////////////////////////////////////////////////////////////////////
	//SystemLog::Impl_
	//////////////////////////////////////////////////////////////////////////
	class standard_log_manager::impl {
	public:
		std::unique_ptr<std::ofstream> out_put_file;
		std::list<std::ostream *> output_streams;
		size_t log_history_max_size = default_log_history_max_size; //ログ履歴最大サイズ(0で無限)
		std::list<std::string> log_history; //ログ履歴
		bool output_console_flag = false;

		static std::string get_string_formatted_by_log_level(const log_level level, const std::string &detail,
		                                                     const std::string &place) {
			using namespace std;
			string header;
			switch (level) {
				case log_level::message:
					header = message_header;
					break;
				case log_level::warning:
					header = warning_header;
					break;
				case log_level::error:
					header = error_header;
					break;
				case log_level::fatal:
					header = fatal_error_header;
					break;
			}
			//出力文字列を生成
			stringstream s_strm;
			//ログのシステム時刻
			const auto &pm = *game::instance().performance_manager();
			s_strm << '[' << pm.get_current_time_count().to_string() << ',' << pm.get_current_frame_count() << ']';
			//ログのレベル
			s_strm << header << ':';
			//詳細
			s_strm << detail;
			//場所
			s_strm << '@' << place;
			return s_strm.str();
		}

		void out_put_to_out_stream(const std::string &str) const {
			const std::string o_str = add_new_line_if_need(str);
			for (const auto &o_strm : output_streams) { *o_strm << o_str; }
		}

		void add_history(const std::string &str) {
			log_history.push_back(str);
			if (log_history_max_size == 0) { return; }
			//ログ履歴の最大値を超えていたら戦闘のログを削除する
			if (log_history.size() > log_history_max_size) { log_history.pop_front(); }
		}

		static void out_put_to_console(const std::string &str, const log_level level) {
			int col = 0;
			switch (level) {
				case log_level::message:
					col = win::console::col_gray;
					break;
				case log_level::warning:
					col = win::console::col_yellow;
					break;
				case log_level::error:
					col = win::console::col_red;
					break;
				case log_level::fatal:
					col = win::console::col_violet;
					break;
			}
			win::console::set_character_color(col);
			const auto o_str = add_new_line_if_need(str);
			//o_str = util::ConvertUTF8ToShiftJIS(o_str);
			std::cout << o_str;
		}

		static void assertion_by_level(const log_level level, [[maybe_unused]] const std::string &err_str) {
			if (level == log_level::fatal) {
				#ifndef NDEBUG
				switch (MessageBox(nullptr, "致命的なエラーが発生しました。ブレークポイントを作成しますか？", "致命的エラー",
				                   MB_YESNO | MB_ICONERROR | MB_APPLMODAL)) {
					case IDYES:
						DebugBreak();
						break;
					case IDNO:
						return;
					default:
						assert(false);
				}
				#endif
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//SystemLog
	//////////////////////////////////////////////////////////////////////////

	standard_log_manager::standard_log_manager() : impl_(std::make_unique<impl>()) {}

	standard_log_manager::~standard_log_manager() = default;

	bool standard_log_manager::initialize() {
		bool debug_mode = false;
		#ifdef _DEBUG
		debug_mode = true;
		#else
			debug_mode = false;
		#endif
		simple_log(std::string("PlanetaEngine v") + private_::system_variables::engine_information::version_string,
		           debug_mode ? " デバッグビルド" : "");
		simple_log(std::string("起動日時:") + util::date_time::get_current_date_time().to_string());
		if (private_::system_variables::development_mode) { simple_log("開発モードが有効です。"); }
		PE_LOG_MESSAGE("ログ出力が開始されました。ログ出力ストリームは", impl_->output_streams.size(), "個です。");
		return true;
	}

	void standard_log_manager::finalize() {
		PE_LOG_MESSAGE("システムログを終了します。");
		reset_log_out_stream();
		if (impl_->output_console_flag) {
			close_console_window();
			impl_->output_console_flag = false;
		}
		impl_->log_history.clear();
		impl_->log_history_max_size = default_log_history_max_size;
		if (impl_->out_put_file) { impl_->out_put_file->close(); }
	}

	bool standard_log_manager::dump_log_history(const std::string &file_name) {
		std::ofstream ofs(file_name);
		if (ofs.bad()) {
			log(log_level::warning, __FUNCTION__, "ログ履歴をファイルに出力できませんでした。ファイル(", file_name, ")が開けません");
			return false;
		}
		for (const auto &str : impl_->log_history) { ofs << str << "\n"; }
		log(log_level::message, __FUNCTION__, "ログ履歴をファイル(", file_name, ")に出力しました。");
		return true;
	}

	void standard_log_manager::out_put(const log_level level, const std::string &str) const {
		impl_->out_put_to_out_stream(str);
		//ログ履歴に追加
		impl_->add_history(str);
		//コンソールに出力
		if (impl_->output_console_flag) { impl_->out_put_to_console(str, level); }
		//デバッグウインドウに出力
		OutputDebugString(add_new_line_if_need(str).c_str());
	}

	void standard_log_manager::add_log_out_stream(std::ostream &o_strm) const {
		impl_->output_streams.push_back(&o_strm);
		//			LogMessage("ログ出力ストリームを追加しました。", "SystemLog::AddLogOutStream");
	}

	void standard_log_manager::set_log_history_max_size(size_t size) {
		impl_->log_history_max_size = size;
		log(log_level::message, __FUNCTION__, "ログ履歴の最大サイズを", size, "に変更しました。");
	}

	bool standard_log_manager::validate_console_out_put() {
		open_console_window();
		impl_->output_console_flag = true;
		simple_log("コンソールへのログ出力を有効化しました。");
		return true;
	}

	void standard_log_manager::log_proc(const log_level level, const std::string &detail, const std::string &place) {
		const std::string str = impl_->get_string_formatted_by_log_level(level, detail, place);
		out_put(level, str);
		impl_->assertion_by_level(level, str);
	}

	void standard_log_manager::reset_log_out_stream() const { impl_->output_streams.clear(); }

	void standard_log_manager::simple_log_proc(const std::string &detail) { out_put(log_level::message, detail); }

	bool standard_log_manager::validate_file_out_put(const std::string &file_name) const {
		if (impl_->out_put_file) { return false; } else {
			impl_->out_put_file = std::make_unique<std::ofstream>();
			impl_->out_put_file->open(file_name, std::ios::out | std::ios::trunc);
			add_log_out_stream(*impl_->out_put_file);
			return true;
		}
	}
}
