#include "file_istream.hpp"
#include "file_stream_buf.hpp"

namespace plnt {
	file_istream::file_istream(file &f) : std::istream(nullptr), stream_buf_(std::make_unique<file_stream_buf>(f)) {
		set_rdbuf(stream_buf_.get());
		clear();
	}

	file_istream::file_istream(const file &f) : file_istream(const_cast<file &>(f)) { }

	file_istream::~file_istream() = default;
}
