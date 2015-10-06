#include "SystemLog.h"
#include "SystemCounter.h"
#include "SystemVariables.h"
#include <sstream>
#include <iostream>
#include "boost/lexical_cast.hpp"
#include "PEDateTime.h"

/*ログの書式
[hh:mm:ss,frame]level:detail
*/

namespace {
	const std::string kMessageHeader("メッセージ");
	const std::string kWarningHeader("警告");
	const std::string kErrorHeader("エラー");
	const size_t kDefaultLogHistoryMaxSize(0);
}

namespace planeta_engine {
	namespace debug {

		bool SystemLog::Initialize()
		{
			_OutPutToOutStream(std::string("PlanetaEngine V") + core::system_variables::system_information::VersionString);
			_OutPutToOutStream(std::string("起動日時:") + utility::DateTime::GetCurrentDateTime().ToString());
			if (core::system_variables::DevelopmentMode)_OutPutToOutStream("開発モードが有効です。");
			char str[256];
			sprintf_s(str, 256, "初期化されました。ログ出力ストリームは%d個です。", (signed)_output_streams.size());
			LogMessage(str, "SystemLog::Initialize()");;
			return true;
		}

		bool SystemLog::Finalize()
		{
			ResetLogOutStream();
			_log_history.clear();
			_log_history_max_size = kDefaultLogHistoryMaxSize;
			LogMessage("終了処理が実行されました。", "SystemLog::Finalize()");
			return true;
		}

		bool SystemLog::DumpLogHistory(const std::string& file_name)
		{
			std::ofstream ofs(file_name);
			if (ofs.bad()) { 
				LogWarning(std::string("ログ履歴をファイルに出力できませんでした。ファイルが開けません") + "(" + file_name + ")", "SystemLog::DumpLogHistory");
				return false; 
			}
			for (const auto& str : _log_history) {
				ofs << str << std::endl;
			}
			LogMessage(std::string("ログ履歴をファイルに出力しました。") + "(" + file_name + ")", "SystemLog::DumpLogHistory");
			return true;
		}

		void SystemLog::_Log(_LogLevel level, const std::string& detail, const std::string& place)
		{
			using namespace std;
			string header;
			switch (level)
			{
			case _LogLevel::Message:
				header = kMessageHeader;
				break;
			case _LogLevel::Warning:
				header = kWarningHeader;
				break;
			case _LogLevel::Error:
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
			//出力
			_OutPutToOutStream(sstrm.str());
			//ログ履歴に追加
			_AddHistory(sstrm.str());
		}

		void SystemLog::_OutPutToOutStream(const std::string& str)
		{
			for (auto& ostrm : _output_streams)
			{
				*ostrm << str << std::endl;
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
			LogMessage(std::string("ログ履歴の最大サイズを変更しました。(") + boost::lexical_cast<std::string>(size) + ")", "SystemLog::SetLogHistoryMaxSize");
		}

	}
}