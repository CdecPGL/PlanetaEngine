#pragma once

#include <string>
#include <list>
#include <fstream>
#include "PointerSingletonTemplate.h"
#include "StringUtility.h"

namespace planeta_engine {
	namespace debug {
		//システムログ管理(スタティックシングルトン)
		class SystemLog : public utility::PointerSingletonTemplate<SystemLog>
		{
			friend utility::PointerSingletonTemplate<SystemLog>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			enum class LogLevel { Message, Warning, Error };
			/*レベルを指定してログを出力(レベル、発生個所、詳細(複数指定することで連結される。))*/
			template<typename... Details>
			void Log(LogLevel level, const std::string& place, Details&&... details) {
				_Log(level, utility::ConvertAndConnectToString(std::forward<Details>(details)...), place);
			}
			/*シンプルなログを出力*/
			template<typename... Details>
			void SimpleLog(Details&&... details) {
				_OutPutToOutStream(utility::ConvertAndConnectToString(std::forward<Details>(details)...));
			}
			/*メッセージ(詳細、発生個所)*/
			void LogMessage(const std::string& detail, const std::string& place) { _Log(LogLevel::Message, detail, place); }
			/*警告(詳細、発生個所)*/
			void LogWarning(const std::string& detail, const std::string& place) { _Log(LogLevel::Warning, detail, place); }
			/*エラー(詳細、発生個所)*/
			void LogError(const std::string& detail, const std::string& place) { _Log(LogLevel::Error, detail, place); }

			/*ログ出力ストリームを追加(インスタンスの生成、破棄は外部で行う)*/
			void AddLogOutStream(std::ostream& ostrm);
			/*ログ出力ストリームをすべて消す*/
			void ResetLogOutStream() { _output_streams.clear(); }
			/*ログ履歴を指定されたファイルに出力(エンジンのファイルシステムは使わず、カレントディレクトリに指定された名前で直接出力する)*/
			bool DumpLogHistory(const std::string& file_name);
			/*ログ履歴最大サイズを設定(0で無限)*/
			void SetLogHistoryMaxSize(size_t size);
		private:
			SystemLog();
			~SystemLog();
			std::list<std::ostream*> _output_streams;
			size_t _log_history_max_size; //ログ履歴最大サイズ(0で無限)
			std::list<std::string> _log_history; //ログ履歴

			void _Log(LogLevel level, const std::string& detail, const std::string& place);
			void _OutPutToOutStream(const std::string& str);
			void _AddHistory(const std::string& str);
		};
		using LogLevel = SystemLog::LogLevel;
	}
}
