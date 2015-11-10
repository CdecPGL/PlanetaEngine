#include "File.h"

namespace planeta_engine{
	namespace file_system{

		File::File()
		{
		}

		File::File(const std::string& ext) :extension_(ext){}

		File::File(const File& obj):extension_(obj.extension_),status_(obj.status_),size_(obj.size_) {
			data_top_ = reinterpret_cast<unsigned char*>(malloc(sizeof(unsigned char)*size_));
			memcpy_s(data_top_, size_, obj.data_top_, obj.size_);
		}

		File::File(File&& obj):extension_(std::move(obj.extension_)),status_(obj.status_),size_(obj.size_),data_top_(obj.data_top_) {
			obj.status_ = FileStatus::Unloaded;
			obj.size_ = 0;
			obj.data_top_ = nullptr;
		}

		File::~File()
		{
			if (data_top_){
				delete[] data_top_;
			}
		}

		File& File::operator=(const File& obj) {
			extension_ = obj.extension_;
			status_ = obj.status_;
			size_ = obj.size_;
			if (data_top_) { free(data_top_); }
			data_top_ = reinterpret_cast<unsigned char*>(malloc(sizeof(unsigned char)*size_));
			memcpy_s(data_top_, size_, obj.data_top_, obj.size_);
			return *this;
		}

		File& File::operator=(File&& obj) {
			extension_ = std::move(obj.extension_);
			status_ = obj.status_;
			size_ = obj.size_;
			if (data_top_) { free(data_top_); }
			data_top_ = obj.data_top_;
			obj.status_ = FileStatus::Unloaded;
			obj.size_ = 0;
			obj.data_top_ = nullptr;
			return *this;
		}

		std::string File::GetExtension()const{ return extension_; }
		unsigned int File::GetSize()const{ return size_; }
		const unsigned char* File::GetTopPointer()const{ return data_top_; }
		unsigned char* File::GetTopPointer(){ return data_top_; }
		File::FileStatus File::GetStatus()const{ return status_; }

		void File::UnloadData(){
			if (data_top_){
				size_ = 0;
				delete[] data_top_;
				data_top_ = nullptr;
				status_ = FileStatus::Unloaded;
			}
		}

		void File::ErrorOccured(){
			status_ = FileStatus::Error;
		}

		bool File::ChangeSize(size_t size,bool copy)
		{
			unsigned char* new_area = new unsigned char[size];
			if (new_area == nullptr) { ErrorOccured(); false; } //メモリの確保に失敗した
			if (data_top_) { //すでにメモリが確保されていたら
				if (copy) { memcpy_s(new_area, size, data_top_, size_); }
				delete[] data_top_;
			}
			data_top_ = new_area;
			status_ = FileStatus::Available;
			size_ = size;
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
			if (data_top_) { free(data_top_); }
			data_top_ = top_ptr;
			size_ = size;
			status_ = FileStatus::Available;
		}

	}
}
