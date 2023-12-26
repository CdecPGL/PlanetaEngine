#include "File.hpp"
#include "boost/filesystem/path.hpp"
#include <cassert>

using namespace boost::filesystem;

namespace plnt {
	File::File() { }

	File::File(unsigned char *top_ptr, size_t size, const std::string &file_name) : file_name_(file_name),
		data_top_(top_ptr), size_(size), is_available_(true) {
		path p{file_name};
		if (p.has_extension()) { extension_ = p.extension().string(); }
	}

	File::File(unsigned char *top_ptr, size_t size) : data_top_(top_ptr), size_(size), is_available_(true) { }

	File::File(const File &obj) : extension_(obj.extension_), is_available_(obj.is_available_), size_(obj.size_),
	                              file_name_(obj.file_name_) {
		data_top_ = reinterpret_cast<unsigned char *>(malloc(sizeof(unsigned char) * size_));
		memcpy_s(data_top_, size_, obj.data_top_, obj.size_);
	}

	File::File(File &&obj) : extension_(std::move(obj.extension_)), is_available_(obj.is_available_), size_(obj.size_),
	                         data_top_(obj.data_top_), file_name_(std::move(obj.file_name_)) {
		obj.is_available_ = false;
		obj.size_ = 0;
		obj.data_top_ = nullptr;
	}

	File::~File() { if (data_top_) { delete[] data_top_; } }

	File &File::operator=(const File &obj) & {
		extension_ = obj.extension_;
		is_available_ = obj.is_available_;
		size_ = obj.size_;
		file_name_ = obj.file_name_;
		if (data_top_) { free(data_top_); }
		data_top_ = reinterpret_cast<unsigned char *>(malloc(sizeof(unsigned char) * size_));
		memcpy_s(data_top_, size_, obj.data_top_, obj.size_);
		return *this;
	}

	File &File::operator=(File &&obj) & {
		extension_ = std::move(obj.extension_);
		is_available_ = obj.is_available_;
		size_ = obj.size_;
		file_name_ = std::move(obj.file_name_);
		if (data_top_) { free(data_top_); }
		data_top_ = obj.data_top_;
		obj.is_available_ = false;
		obj.size_ = 0;
		obj.data_top_ = nullptr;
		return *this;
	}

	unsigned int File::size() const { return size_; }
	const unsigned char *File::top_pointer() const & { return data_top_; }
	unsigned char *File::top_pointer() & { return data_top_; }
	bool File::is_available() const { return is_available_; }

	void File::Clear() {
		if (data_top_) {
			size_ = 0;
			delete[] data_top_;
			data_top_ = nullptr;
			is_available_ = false;
		}
	}

	void File::SetFileName(const std::string &file_name) {
		file_name_ = file_name;
		path p{file_name};
		if (p.has_extension()) { extension_ = p.extension().string(); } else { extension_ = ""; }
	}

	std::string File::file_name() const { return file_name_; }

	bool File::Reserve(size_t size, bool copy) {
		unsigned char *new_area = new unsigned char[size];
		//メモリの確保に失敗した
		if (new_area == nullptr) {
			is_available_ = false;
			return false;
		}
		if (data_top_) {
			//すでにメモリが確保されていたら
			if (copy) { memcpy_s(new_area, size, data_top_, size_); }
			delete[] data_top_;
		}
		data_top_ = new_area;
		is_available_ = true;
		size_ = size;
		return true;
	}

	bool File::WriteData(size_t pos, const unsigned char *data_top, size_t data_size, bool auto_extend /*= false*/) {
		//データサイズがファイルサイズより大きかったら
		if (size() < pos + data_size) {
			//自動拡張が有効だったらサイズを増やす
			if (auto_extend) { Reserve(pos + data_size, true); } else { return false; } //無効だったらエラー
		}
		memcpy_s(top_pointer() + pos, size_ - pos, data_top, data_size);
		return true;
	}

	bool File::ReadData(size_t pos, unsigned char *buffer_top, size_t buffer_size) const {
		if (size() < pos + buffer_size) {
			//バッファ長がファイルサイズより大きかったら、ファイル終端までをバッファにコピー
			memcpy_s(buffer_top, buffer_size, top_pointer() + pos, size() - pos);
		} else {
			//バッファ長が短かったらバッファ一杯にコピー
			memcpy_s(buffer_top, buffer_size, top_pointer() + pos, buffer_size);
		}
		return true;
	}

	void File::MoveData(unsigned char *top_ptr, size_t size) {
		if (data_top_) { free(data_top_); }
		data_top_ = top_ptr;
		size_ = size;
		is_available_ = true;
	}

	void File::SetData(const std::string &data) {
		Reserve(data.size(), false);
		bool ret = WriteData(0, reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), false);
		assert(ret == true);
	}
}
