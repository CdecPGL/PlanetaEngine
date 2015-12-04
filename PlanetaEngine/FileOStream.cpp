#include "FileOStream.h"
#include "FileStreamBuf.h"

namespace planeta_engine {
	namespace file_system {

		FileOStream::FileOStream(File& file):stream_buf_(std::make_unique<FileStreamBuf>(file)), std::ostream(stream_buf_.get())
		{
		}

		FileOStream::~FileOStream() = default;

	}
}