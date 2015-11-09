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
			File(const File&);
			File(File&&);
			/*拡張子セット版コンストラクタ*/
			File(const std::string&);
			~File();
			File& operator=(const File&);
			File& operator=(File&&);
			std::string GetExtension()const;
			void UnloadData();
			unsigned int GetSize()const;
			const unsigned char* GetTopPointer()const;
			unsigned char* GetTopPointer();
			FileStatus GetStatus()const;
			/*データのセット(コピーするのではなく管理を委譲する)*/
			void SetData(unsigned char* top_ptr, size_t size);
			/*データの書き込み*/
			bool WriteData(size_t pos, unsigned char* data_top, size_t data_size,bool auto_extend = false);
			template<typename T>
			bool WriteData(size_t pos,const T& d,bool auto_extend = true) {
				return WriteData(pos, reinterpret_cast<unsigned char*>(&T), sizeof(T), auto_extend);
			}
			/*データの読み込み*/
			bool ReadData(size_t pos, unsigned char* buffer_top, size_t buffer_size)const;
			template<typename T>
			bool ReadData(size_t pos,T& b) {
				return ReadData(pos, reinterpret_cast<unsigned char*>(&T), sizeof(T));
			}
			/*サイズの変更*/
			bool ChangeSize(size_t size,bool copy = true);
			/*エラー発生*/
			void ErrorOccured();

		private:
			std::string extension_;
			FileStatus status_ = FileStatus::Unloaded;
			unsigned char* data_top_ = nullptr;
			unsigned int size_ = 0;
		};
	}
}
