#pragma once

#include <istream>
#include <memory>

namespace plnt {
	class file;
	class file_stream_buf;

	class file_istream final : public std::istream {
	public:
		file_istream(const file_istream &) = delete;
		file_istream(file_istream &&) = delete;
		explicit file_istream(file &f);
		explicit file_istream(const file &f);
		~file_istream() override;
		file_istream &operator=(const file_istream &) = delete;
		file_istream &operator=(file_istream &&) = delete;

	private:
		std::unique_ptr<file_stream_buf> stream_buf_;
	};
}
