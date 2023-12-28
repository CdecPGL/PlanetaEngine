#pragma once

#include <ostream>
#include <memory>

namespace plnt {
	class file_stream_buf;
	class file;

	class file_ostream final : public std::ostream {
	public:
		file_ostream(const file_ostream &) = delete;
		file_ostream(file_ostream &&) = delete;
		explicit file_ostream(file &f);
		~file_ostream() override;
		file_ostream &operator=(const file_ostream &) = delete;
		file_ostream &operator=(file_ostream &&) = delete;

	private:
		std::unique_ptr<file_stream_buf> stream_buf_;
	};
}
