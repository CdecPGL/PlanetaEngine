﻿#include "FileOStream.hpp"
#include "FileStreamBuf.hpp"

namespace plnt {
	FileOStream::FileOStream(File& file) :stream_buf_(std::make_unique<FileStreamBuf>(file)), std::ostream(stream_buf_.get()) {}

	FileOStream::~FileOStream() = default;
}