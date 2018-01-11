#pragma once

#include <string>
#include <list>
#include <fstream>
#include <memory>
#include <cstdio>
#include "LogManager.hpp"

namespace plnt {
	namespace private_ {
		//システムログ管理(スタティックシングルトン)
		class StandardLogManager final: public LogManager
		{
		public:
			StandardLogManager();
			~StandardLogManager();
			bool Initialize()override;
			void Finalize()override;
			/*コンソールへの出力を有効化*/
			bool ValidateConsoleOutPut();
			/*ファイルへの出力を追加*/
			bool ValidateFileOutPut(const std::string& file_name);

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
			class Impl_;
			std::unique_ptr<Impl_> impl_;

			void LogProc(LogLevel level, const std::string& detail, const std::string& place)override;
			void SimpleLogProc(const std::string& detail)override;
			void _OutPut(LogLevel level, const std::string& str);
		};
	}
}

