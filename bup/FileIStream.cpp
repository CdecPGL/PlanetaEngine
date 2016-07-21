#include "FileIStream.h"

#include "FileStreamBuf.h"

namespace planeta {
	FileIStream::FileIStream(File& file) :stream_buf_(std::make_unique<FileStreamBuf>(file)), std::istream(nullptr) {
		set_rdbuf(stream_buf_.get());
		clear();
	}
	FileIStream::FileIStream(const File& file) : FileIStream(const_cast<File&>(file)) {}
	FileIStream::~FileIStream() = default;
}