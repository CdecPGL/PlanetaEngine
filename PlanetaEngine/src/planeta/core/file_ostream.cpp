#include "file_ostream.hpp"
#include "file_stream_buf.hpp"

namespace plnt {
	file_ostream::file_ostream(file &f)
		: std::ostream(new file_stream_buf(f)),
		  stream_buf_(std::unique_ptr<file_stream_buf>(reinterpret_cast<file_stream_buf *>(rdbuf()))) { }

	file_ostream::~file_ostream() = default;
}
