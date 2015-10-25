#pragma once

#include "Object.h"
#include<string>
#include<vector>
#include <memory.h>

namespace planeta_engine{
	namespace file_system{
		class File : public core::Object
		{
		public:
			/*ファイルの状態(利用可能、暗号化されている、ロードされていない、問題発生)*/
			enum class FileStatus { Available, Encrypted, Unloaded, Error };
			File();
			/*拡張子セット版コンストラクタ*/
			File(const std::string&);
			~File();
			std::string GetExtension()const;
			void UnloadData();
			unsigned int GetSize()const;
			const void* const GetTopPointer()const;
			FileStatus GetStatus()const;
			/*データセット(先頭アドレス、サイズ、暗号化データか)*/
			void SetData(void*, unsigned int, bool = false);
			/*エラー発生*/
			void ErrorOccured();

		private:
			const std::string extension;
			FileStatus status = FileStatus::Unloaded;
			void* data_top = nullptr;
			unsigned int size = 0;
		};
	}
}
