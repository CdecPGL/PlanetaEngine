#include "SystemLog.h"
#include "SystemCounter.h"
#include "SystemVariables.h"
#include <sstream>
#include <iostream>
#include <windows.h>
#include <cassert>
#include "PEDateTime.h"
#include "WindowsUtility.h"

/*ログの書式
[hh:mm:ss,frame]level:detail@place
*/

namespace {
	constexpr char* kMessageHeader("メッセージ");
	constexpr char* kWarningHeader("警告");
	constexpr char* kErrorHeader("エラー");
	constexpr size_t kDefaultLogHistoryMaxSize(0);

	void OpenConsoleWindow() {
		AllocConsole();
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONIN$", "r", stdin);
	}
	void CloseConsoleWindow() {
		FreeConsole();
	}

	std::string AddNewLineIfNeed(const std::string& str) {
		std::string out = str;
		if (str.size() > 0 && str[str.size() - 1] != '\n') {
			out += '\n';
		}
		return std::move(out);
	}
}

namespace planeta_engine {
	namespace debug {

		bool SystemLog::Initialize()
		{
			bool debug_mode = false;
#ifdef _DEBUG
			debug_mode = true;
#else
			debug_mode = false;
#endif
			SimpleLog(std::string("PlanetaEngine v") + core::system_variables::engine_information::VersionString, debug_mode ? " デバッグビルド" : "");
			SimpleLog(std::string("起動日時:") + utility::DateTime::GetCurrentDateTime().ToString());
			if (core::system_variables::DevelopmentMode) { SimpleLog("開発モードが有効です。"); }
			Log(LogLevel::Message, __FUNCTION__, "初期化されました。ログ出力ストリームは", _output_streams.size(), "個です。");
			return true;
		}

		bool SystemLog::Finalize()
		{
			LogMessage("終了処理が実行されました。", __FUNCTION__);
			ResetLogOutStream();
			if (output_console_flag_) {
				CloseConsoleWindow();
				output_console_flag_ = false;
			}
			_log_history.clear();
			_log_history_max_size = kDefaultLogHistoryMaxSize;
			return true;
		}

		bool SystemLog::DumpLogHistory(const std::string& file_name)
		{
			std::ofstream ofs(file_name);
			if (ofs.bad()) { 
				Log(LogLevel::Warning, __FUNCTION__, "ログ履歴をファイルに出力できませんでした。ファイル(", file_name, ")が開けません");
				return false;
			}
			for (const auto& str : _log_history) {
				ofs << str << std::endl;
			}
			Log(LogLevel::Message, __FUNCTION__, "ログ履歴をファイル(", file_name, ")に出力しました。");
			return true;
		}

		std::string SystemLog::_GetStringFormatedByLogLevel(LogLevel level, const std::string& detail, const std::string& place)
		{
			using namespace std;
			string header;
			switch (level)
			{
			case LogLevel::Message:
				header = kMessageHeader;
				break;
			case LogLevel::Warning:
				header = kWarningHeader;
				break;
			case LogLevel::Error:
				header = kErrorHeader;
				break;
			default:
				break;
			}
			//出力文字列を生成
			stringstream sstrm;
			//ログのシステム時刻
			sstrm << '[' << SystemCounter::instance().GetCurrentTimeCount().ToString() << ',' << SystemCounter::instance().GetCurrentFrameCount() << ']';
			//ログのレベル
			sstrm << header << ':';
			//詳細
			sstrm << detail;
			//場所
			sstrm << '@' << place;
			return std::move(sstrm.str());
		}

		void SystemLog::_OutPut(LogLevel level, const std::string& str) {
			_OutPutToOutStream(str);
			//ログ履歴に追加
			_AddHistory(str);
			//コンソールに出力
			if (output_console_flag_) { _OutPutToConsole(str, level); }
			//デバッグウインドウに出力
			OutputDebugString(AddNewLineIfNeed(str).c_str());
		}

		void SystemLog::_OutPutToOutStream(const std::string& str)
		{
			std::string ostr = AddNewLineIfNeed(str);
			for (auto& ostrm : _output_streams)
			{
				*ostrm << ostr;
			}
		}

		SystemLog::SystemLog():_log_history_max_size(kDefaultLogHistoryMaxSize)
		{

		}

		SystemLog::~SystemLog()
		{

		}

		void SystemLog::_AddHistory(const std::string& str)
		{
			_log_history.push_back(str);
			if (_log_history_max_size == 0) { return; }
			//ログ履歴の最大値を超えていたら戦闘のログを削除する
			if (_log_history.size() > _log_history_max_size) { _log_history.pop_front(); }
		}

		void SystemLog::AddLogOutStream(std::ostream& ostrm)
		{
			_output_streams.push_back(&ostrm);
//			LogMessage("ログ出力ストリームを追加しました。", "SystemLog::AddLogOutStream");
		}

		void SystemLog::SetLogHistoryMaxSize(size_t size)
		{
			_log_history_max_size = size;
			Log(LogLevel::Message, __FUNCTION__, "ログ履歴の最大サイズを", size, "に変更しました。");
		}

		bool SystemLog::ValidateConsoleOutPut() {
			OpenConsoleWindow();
			output_console_flag_ = true;
			SimpleLog("コンソールへのログ出力を有効化しました。");
			return true;
		}

		void SystemLog::_OutPutToConsole(const std::string& str, LogLevel level) {
			int col;
			switch (level) {
			case planeta_engine::debug::SystemLog::LogLevel::Message:
				col = windows::console::COL_GRAY;
				break;
			case planeta_engine::debug::SystemLog::LogLevel::Warning:
				col = windows::console::COL_YELLOW;
				break;
			case planeta_engine::debug::SystemLog::LogLevel::Error:
				col = windows::console::COL_RED;
				break;
			default:
				assert(false);
				break;
			}
			windows::console::SetCharacterColor(col);
			std::cout << AddNewLineIfNeed(str);
		}

	}
}