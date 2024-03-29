﻿#pragma once

#include <streambuf>
#include <memory>

namespace plnt {
	class file;

	class file_stream_buf final : public std::streambuf {
	public:
		explicit file_stream_buf(file &file);

	private:
		file &file_;
		int_type overflow(int_type = std::char_traits<char>::eof()) override;
		int_type underflow() override;
		int sync() override;
		pos_type seekoff(off_type, std::ios_base::seekdir,
		                 std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;
		int_type uflow() override;
		pos_type seekpos(pos_type, std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;

		bool save_data_from_write_buffer(size_t block, pos_type buf_off);

		static const size_t buffer_size;
		std::unique_ptr<char[]> write_buffer_;
		size_t writing_block_ = 0;
	};
}
