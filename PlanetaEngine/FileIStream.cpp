#include "FileIStream.h"

#include "FileStreamBuf.h"

namespace planeta_engine {
	namespace file_system {

		FileIStream::FileIStream(File& file):stream_buf_(std::make_unique<FileStreamBuf>(file)),std::istream(stream_buf_.get())
		{
		}
		FileIStream::~FileIStream() = default;
	}
}