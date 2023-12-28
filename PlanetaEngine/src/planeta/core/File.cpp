#include "file.hpp"
#include "boost/filesystem/path.hpp"
#include <cassert>

using namespace boost::filesystem;

namespace plnt {
	file::file() = default;

	file::file(unsigned char *top_ptr, const size_t size, const std::string &file_name) : is_available_(true),
		data_top_(top_ptr), size_(size), file_name_(file_name) {
		if (const path p{file_name}; p.has_extension()) { extension_ = p.extension().string(); }
	}

	file::file(unsigned char *top_ptr, const size_t size) : is_available_(true), data_top_(top_ptr), size_(size) { }

	file::file(const file &obj) : extension_(obj.extension_), is_available_(obj.is_available_), size_(obj.size_),
	                              file_name_(obj.file_name_) {
		data_top_ = static_cast<unsigned char *>(malloc(sizeof(unsigned char) * size_));
		memcpy_s(data_top_, size_, obj.data_top_, obj.size_);
	}

	file::file(file &&obj) noexcept : extension_(std::move(obj.extension_)), is_available_(obj.is_available_),
	                                  data_top_(obj.data_top_),
	                                  size_(obj.size_), file_name_(std::move(obj.file_name_)) {
		obj.is_available_ = false;
		obj.size_ = 0;
		obj.data_top_ = nullptr;
	}

	file::~file() { delete[] data_top_; }

	file &file::operator=(const file &obj) & {
		if (this == &obj) { return *this; }

		extension_ = obj.extension_;
		is_available_ = obj.is_available_;
		size_ = obj.size_;
		file_name_ = obj.file_name_;
		if (data_top_) { free(data_top_); }
		data_top_ = static_cast<unsigned char *>(malloc(sizeof(unsigned char) * size_));
		memcpy_s(data_top_, size_, obj.data_top_, obj.size_);
		return *this;
	}

	file &file::operator=(file &&obj) & noexcept {
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

	size_t file::size() const { return size_; }
	const unsigned char *file::top_pointer() const & { return data_top_; }
	unsigned char *file::top_pointer() & { return data_top_; }
	bool file::is_available() const { return is_available_; }

	void file::clear() {
		if (data_top_) {
			size_ = 0;
			delete[] data_top_;
			data_top_ = nullptr;
			is_available_ = false;
		}
	}

	void file::set_file_name(const std::string &file_name) {
		file_name_ = file_name;
		if (const path p{file_name}; p.has_extension()) { extension_ = p.extension().string(); } else {
			extension_ = "";
		}
	}

	std::string file::file_name() const { return file_name_; }

	bool file::reserve(const size_t size, const bool copy) {
		const auto new_area = new unsigned char[size];
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

	bool file::write_data(const size_t pos, const unsigned char *data_top, const size_t data_size,
	                      const bool auto_extend /*= false*/) {
		//データサイズがファイルサイズより大きかったら
		if (size() < pos + data_size) {
			//自動拡張が有効だったらサイズを増やす
			if (auto_extend) { reserve(pos + data_size, true); } else { return false; } //無効だったらエラー
		}
		memcpy_s(top_pointer() + pos, size_ - pos, data_top, data_size);
		return true;
	}

	bool file::read_data(const size_t pos, unsigned char *buffer_top, const size_t buffer_size) const {
		if (size() < pos + buffer_size) {
			//バッファ長がファイルサイズより大きかったら、ファイル終端までをバッファにコピー
			memcpy_s(buffer_top, buffer_size, top_pointer() + pos, size() - pos);
		} else {
			//バッファ長が短かったらバッファ一杯にコピー
			memcpy_s(buffer_top, buffer_size, top_pointer() + pos, buffer_size);
		}
		return true;
	}

	void file::move_data(unsigned char *top_ptr, const size_t size) {
		if (data_top_) { free(data_top_); }
		data_top_ = top_ptr;
		size_ = size;
		is_available_ = true;
	}

	void file::set_data(const std::string &data) {
		reserve(data.size(), false);
		const bool ret = write_data(0, reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), false);
		assert(ret == true);
	}
}
