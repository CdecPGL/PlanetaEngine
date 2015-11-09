#include "File.h"

namespace planeta_engine{
	namespace file_system{

		File::File()
		{
		}

		File::File(const std::string& ext) :extension(ext){}


		File::~File()
		{
			if (data_top){
				delete[] data_top;
			}
		}

		std::string File::GetExtension()const{ return extension; }
		unsigned int File::GetSize()const{ return size_; }
		const unsigned char* File::GetTopPointer()const{ return data_top; }
		unsigned char* File::GetTopPointer(){ return data_top; }
		File::FileStatus File::GetStatus()const{ return status; }

		void File::UnloadData(){
			if (data_top){
				size_ = 0;
				delete[] data_top;
				data_top = nullptr;
				status = FileStatus::Unloaded;
			}
		}

		void File::ErrorOccured(){
			status = FileStatus::Error;
		}

		bool File::ChangeSize(size_t size,bool copy)
		{
			unsigned char* new_area = new unsigned char[size];
			if (new_area == nullptr) { ErrorOccured(); false; } //メモリの確保に失敗した
			if (data_top) { //すでにメモリが確保されていたら
				if (copy) { memcpy_s(new_area, size, data_top, size_); }
				delete[] data_top;
			}
			data_top = new_area;
			status = FileStatus::Available;
			return true;
		}

		bool File::WriteData(size_t pos, unsigned char* data_top, size_t data_size, bool auto_extend /*= false*/)
		{
			//データサイズがファイルサイズより大きかったら
			if (GetSize() < pos + data_size) { //自動拡張が有効だったらサイズを増やす
				if (auto_extend) { ChangeSize(pos + data_size, true); }
				else { return false; } //無効だったらエラー
			}
			memcpy_s(GetTopPointer() + pos, size_ - pos, data_top, data_size);
			return true;
		}

		bool File::ReadData(size_t pos, unsigned char* buffer_top, size_t buffer_size) const
		{
			if (GetSize() < pos + buffer_size) {
				//バッファ長がファイルサイズより大きかったら、ファイル終端までをバッファにコピー
				memcpy_s(buffer_top, buffer_size, GetTopPointer() + pos, GetSize() - pos);
			}
			else {
				//バッファ長が短かったらバッファ一杯にコピー
				memcpy_s(buffer_top, buffer_size, GetTopPointer() + pos, buffer_size);
			}
			return true;
		}

		void File::SetData(unsigned char* top_ptr, size_t size)
		{
			if (data_top) { free(data_top); }
			data_top = top_ptr;
			size_ = size;
			status = FileStatus::Available;
		}

	}
}
