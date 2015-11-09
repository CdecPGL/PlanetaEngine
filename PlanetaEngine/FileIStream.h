#pragma once

#include <istream>
#include <memory>

namespace planeta_engine {
	namespace file_system {
		class File;
		class FileStreamBuf;
		class FileIStream : public std::istream {
		public:
			FileIStream(File& file);
			~FileIStream();
		private:
			std::unique_ptr<FileStreamBuf> stream_buf_;
		};
	}
}