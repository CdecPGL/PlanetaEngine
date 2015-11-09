#pragma once

#include <ostream>
#include <memory>

namespace planeta_engine {
	namespace file_system {
		class FileStreamBuf;
		class File;
		class FileOStream final: public std::ostream {
		public:
			FileOStream(File& file);
			~FileOStream();
		private:
			std::unique_ptr<FileStreamBuf> stream_buf_;
		};
	}
}