#include <fstream>
#include "game.hpp"
#include "i_performance_manager.hpp"
#include "StandardLogManager.hpp"
#include "SystemVariables.hpp"
#include <sstream>
#include <iostream>
#include <windows.h>
#include <cassert>
#include "date_time.hpp"
#include "..\win\windows_utility.hpp"
#include "character_code.hpp"
#include "log_utility.hpp"

/*ログの書式
[hh:mm:ss,frame]level:detail@place
*/

namespace {
	constexpr char *kMessageHeader(const_cast<char *>("M"));
	constexpr char *kWarningHeader(const_cast<char *>("W"));
	constexpr char *kErrorHeader(const_cast<char *>("E"));
	constexpr char *kFatalErrorHeader(const_cast<char *>("F"));
	constexpr size_t kDefaultLogHistoryMaxSize(100);

	void OpenConsoleWindow() {
		AllocConsole();
		FILE *fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONIN$", "r", stdin);
	}

	void CloseConsoleWindow() { FreeConsole(); }

	std::string AddNewLineIfNeed(const std::string &str) {
		std::string out = str;
		if (str.size() > 0 && str[str.size() - 1] != '\n') { out += '\n'; }
		return std::move(out);
	}
}

namespace plnt {
	namespace private_ {
		//////////////////////////////////////////////////////////////////////////
		//SystemLog::Impl_
		//////////////////////////////////////////////////////////////////////////
		class StandardLogManager::Impl_ {
		public:
			Impl_(StandardLogManager &ins) : ins_(ins) { }

			~Impl_() = default;

			std::unique_ptr<std::ofstream> out_put_file_;
			std::list<std::ostream *> _output_streams;
			size_t _log_history_max_size = kDefaultLogHistoryMaxSize; //ログ履歴最大サイズ(0で無限)
			std::list<std::string> _log_history; //ログ履歴
			bool output_console_flag_ = false;

			static std::string _GetStringFormatedByLogLevel(log_level level, const std::string &detail,
			                                                const std::string &place) {
				using namespace std;
				string header;
				switch (level) {
					case log_level::message:
						header = kMessageHeader;
						break;
					case log_level::warning:
						header = kWarningHeader;
						break;
					case log_level::error:
						header = kErrorHeader;
						break;
					case log_level::fatal:
						header = kFatalErrorHeader;
						break;
					default:
						break;
				}
				//出力文字列を生成
				stringstream sstrm;
				//ログのシステム時刻
				auto &pm = *game::instance().performance_manager();
				sstrm << '[' << pm.get_current_time_count().ToString() << ',' << pm.get_current_frame_count() << ']';
				//ログのレベル
				sstrm << header << ':';
				//詳細
				sstrm << detail;
				//場所
				sstrm << '@' << place;
				return std::move(sstrm.str());
			}

			void _OutPutToOutStream(const std::string &str) {
				std::string ostr = AddNewLineIfNeed(str);
				for (auto &ostrm : _output_streams) { *ostrm << ostr; }
			}

			void _AddHistory(const std::string &str) {
				_log_history.push_back(str);
				if (_log_history_max_size == 0) { return; }
				//ログ履歴の最大値を超えていたら戦闘のログを削除する
				if (_log_history.size() > _log_history_max_size) { _log_history.pop_front(); }
			}

			void _OutPutToConsole(const std::string &str, log_level level) {
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
					default:
						assert(false);
						break;
				}
				win::console::set_character_color(col);
				auto ostr = AddNewLineIfNeed(str);
				//ostr = util::ConvertUTF8ToShiftJIS(ostr);
				std::cout << ostr;
			}

			void _AssertionByLevel(log_level level, const std::string err_str) {
				if (level == log_level::fatal) {
					#ifndef NDEBUG
					int ret = MessageBox(NULL, "致命的なエラーが発生しました。ブレークポイントを作成しますか？", "致命的エラー",
					                     MB_YESNO | MB_ICONERROR | MB_APPLMODAL);
					switch (ret) {
						case IDYES:
							DebugBreak();
							break;
						case IDNO:
							return;
						default:
							assert(false);
							return;
					}
					#endif
				}
			}

		private:
			StandardLogManager &ins_;
		};

		//////////////////////////////////////////////////////////////////////////
		//SystemLog
		//////////////////////////////////////////////////////////////////////////

		StandardLogManager::StandardLogManager() : impl_(std::make_unique<Impl_>(*this)) { }

		StandardLogManager::~StandardLogManager() = default;

		bool StandardLogManager::initialize() {
			bool debug_mode = false;
			#ifdef _DEBUG
			debug_mode = true;
			#else
			debug_mode = false;
			#endif
			simple_log(std::string("PlanetaEngine v") + private_::system_variables::engine_information::VersionString,
			          debug_mode ? " デバッグビルド" : "");
			simple_log(std::string("起動日時:") + util::date_time::get_current_date_time().ToString());
			if (private_::system_variables::DevelopmentMode) { simple_log("開発モードが有効です。"); }
			PE_LOG_MESSAGE("ログ出力が開始されました。ログ出力ストリームは", impl_->_output_streams.size(), "個です。");
			return true;
		}

		void StandardLogManager::finalize() {
			PE_LOG_MESSAGE("システムログを終了します。");
			ResetLogOutStream();
			if (impl_->output_console_flag_) {
				CloseConsoleWindow();
				impl_->output_console_flag_ = false;
			}
			impl_->_log_history.clear();
			impl_->_log_history_max_size = kDefaultLogHistoryMaxSize;
			if (impl_->out_put_file_) { impl_->out_put_file_->close(); }
		}

		bool StandardLogManager::DumpLogHistory(const std::string &file_name) {
			std::ofstream ofs(file_name);
			if (ofs.bad()) {
				log(log_level::warning, __FUNCTION__, "ログ履歴をファイルに出力できませんでした。ファイル(", file_name, ")が開けません");
				return false;
			}
			for (const auto &str : impl_->_log_history) { ofs << str << std::endl; }
			log(log_level::message, __FUNCTION__, "ログ履歴をファイル(", file_name, ")に出力しました。");
			return true;
		}

		void StandardLogManager::_OutPut(log_level level, const std::string &str) {
			impl_->_OutPutToOutStream(str);
			//ログ履歴に追加
			impl_->_AddHistory(str);
			//コンソールに出力
			if (impl_->output_console_flag_) { impl_->_OutPutToConsole(str, level); }
			//デバッグウインドウに出力
			OutputDebugString(AddNewLineIfNeed(str).c_str());
		}

		void StandardLogManager::AddLogOutStream(std::ostream &ostrm) {
			impl_->_output_streams.push_back(&ostrm);
			//			LogMessage("ログ出力ストリームを追加しました。", "SystemLog::AddLogOutStream");
		}

		void StandardLogManager::SetLogHistoryMaxSize(size_t size) {
			impl_->_log_history_max_size = size;
			log(log_level::message, __FUNCTION__, "ログ履歴の最大サイズを", size, "に変更しました。");
		}

		bool StandardLogManager::ValidateConsoleOutPut() {
			OpenConsoleWindow();
			impl_->output_console_flag_ = true;
			simple_log("コンソールへのログ出力を有効化しました。");
			return true;
		}

		void StandardLogManager::log_proc(log_level level, const std::string &detail, const std::string &place) {
			std::string str = std::move(impl_->_GetStringFormatedByLogLevel(level, detail, place));
			_OutPut(level, str);
			impl_->_AssertionByLevel(level, str);
		}

		void StandardLogManager::ResetLogOutStream() { impl_->_output_streams.clear(); }

		void StandardLogManager::simple_log_proc(const std::string &detail) { _OutPut(log_level::message, detail); }

		bool StandardLogManager::ValidateFileOutPut(const std::string &file_name) {
			if (impl_->out_put_file_) { return false; } else {
				impl_->out_put_file_ = std::make_unique<std::ofstream>();
				impl_->out_put_file_->open(file_name, std::ios::out | std::ios::trunc);
				AddLogOutStream(*impl_->out_put_file_);
				return true;
			}
		}
	}
}
