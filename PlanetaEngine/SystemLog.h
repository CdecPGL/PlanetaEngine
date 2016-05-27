#pragma once

#include <string>
#include <list>
#include <fstream>
#include <memory>
#include <cstdio>
#include "SingletonTemplate.h"
#include "StringUtility.h"

namespace planeta_engine {
	namespace debug {
		//システムログ管理(スタティックシングルトン)
		class SystemLog : public utility::SingletonTemplate<SystemLog>
		{
			friend utility::SingletonTemplate<SystemLog>;
		public:
			bool Initialize()override;
			void Finalize()override;
			enum class LogLevel { Message, Warning, Error, Fatal };
			/*コンソールへの出力を有効化*/
			bool ValidateConsoleOutPut();
			/*レベルを指定してログを出力(レベル、発生個所、詳細(複数指定することで連結される。))*/
			template<typename... Details>
			void Log(LogLevel level, const std::string& place, Details&&... details) {
				_Log(level, utility::ConvertAndConnectToString(std::forward<Details>(details)...), place);
			}
			/*シンプルなログを出力*/
			template<typename... Details>
			void SimpleLog(Details&&... details) {
				_OutPut(LogLevel::Message, utility::ConvertAndConnectToString(std::forward<Details>(details)...));
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
			void ResetLogOutStream();
			/*ログ履歴を指定されたファイルに出力(エンジンのファイルシステムは使わず、カレントディレクトリに指定された名前で直接出力する)*/
			bool DumpLogHistory(const std::string& file_name);
			/*ログ履歴最大サイズを設定(0で無限)*/
			void SetLogHistoryMaxSize(size_t size);
			enum class DumpLevel { All, Message, Warning, Error };
			/*出力レベルを設定*/
			void SetDumpLevel();
		private:
			SystemLog();
			~SystemLog();
			class Impl_;
			std::unique_ptr<Impl_> impl_;

			void _Log(LogLevel level, const std::string& detail, const std::string& place);
			void _OutPut(LogLevel level, const std::string& str);
		};
		using LogLevel = SystemLog::LogLevel;
	}
}

//メッセージを出力する
#define PE_LOG_MESSAGE(...)\
planeta_engine::debug::SystemLog::instance().Log(debug::LogLevel::Message,__FUNCTION__,__VA_ARGS__)
//警告を出力する
#define PE_LOG_WARNING(...)\
planeta_engine::debug::SystemLog::instance().Log(debug::LogLevel::Warning,__FUNCTION__,__VA_ARGS__)
//エラーを出力する
#define PE_LOG_ERROR(...)\
planeta_engine::debug::SystemLog::instance().Log(debug::LogLevel::Error,__FUNCTION__,__VA_ARGS__)
//致命的エラーを出力する(デバッグモードの時は、プログラムを停止させる)
#define PE_LOG_FATAL(...)\
planeta_engine::debug::SystemLog::instance().Log(debug::LogLevel::Fatal,__FUNCTION__,__VA_ARGS__)
//条件チェック(引数がfalseなら致命的エラーを発生させる)
#define PE_VERIFY(condition)\
if((condition)==false){planeta_engine::debug::SystemLog::instance().Log(debug::LogLevel::Fatal,__FUNCTION__,", expression : ",#condition);}
