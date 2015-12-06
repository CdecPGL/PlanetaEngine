#pragma once

#include <streambuf>
#include <memory>

namespace planeta_engine {
	namespace file_system {
		class File;
		class FileStreamBuf : public std::streambuf {
		public:
			FileStreamBuf(File& file);
		private:
			File& file_;
			int_type overflow(int_type = std::char_traits<char>::eof()) override;
			int_type underflow() override;
			int sync() override;
			pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;
			int_type uflow() override;
			pos_type seekpos(pos_type, std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;

			bool SaveDataFromWriteBuffer(size_t block, pos_type buf_off);

			static const size_t buffer_size;
			std::unique_ptr<char[]> write_buffer_;
			size_t writing_block_ = 0;
		};
	}
}