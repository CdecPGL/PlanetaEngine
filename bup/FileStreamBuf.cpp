#include "FileStreamBuf.h"
#include "File.h"

namespace planeta {
	const size_t FileStreamBuf::buffer_size = 256;

	FileStreamBuf::FileStreamBuf(File& file) :file_(file), write_buffer_(std::make_unique<char[]>(buffer_size)) {
		write_buffer_[0] = (char)std::char_traits<char>::eof();
		char* top = reinterpret_cast<char*>(file.GetTopPointer());
		setg(top, top, top + file.GetSize());
	}

	FileStreamBuf::int_type FileStreamBuf::overflow(int_type d/*= char_traits<char>::eof()*/) {
		SaveDataFromWriteBuffer(writing_block_++, 1);
		//あふれた1バイト分書き込んむ
		write_buffer_[0] = (char)d;
		return d;
	}

	FileStreamBuf::int_type FileStreamBuf::underflow() {
		return std::char_traits<char>::eof();
	}

	FileStreamBuf::int_type FileStreamBuf::uflow() {
		return std::char_traits<char>::eof();
	}

	int FileStreamBuf::sync() {
		//バッファに書き込みがあった分だけコピーし、リセット
		return SaveDataFromWriteBuffer(writing_block_, pptr() - pbase()) ? 0 : -1;
	}

	FileStreamBuf::pos_type FileStreamBuf::seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode mode/*= ios_base::in | ios_base::out*/) {
		size_t buf_pos;
		pos_type way = dir == std::ios_base::beg ? 0 : file_.GetSize() - 1;
		switch (mode & (std::ios_base::in | std::ios_base::out)) {
		case std::ios_base::out:
			//書き込み位置
			buf_pos = pptr() - pbase();
			if (dir == std::ios_base::cur) { way = writing_block_*buffer_size + buf_pos; }
			if (buf_pos + off < 0 || buf_pos + off >= buffer_size) {
				SaveDataFromWriteBuffer(writing_block_, 0); //現在のバッファをファイルに書き込み
				auto new_write_pos = (int64_t)writing_block_ * buffer_size + buf_pos + off; //新しい書き込み位置
				if (new_write_pos < 0) {
					return std::char_traits<char>::eof();
				} //負だったらだめ
				auto new_buf_pos = new_write_pos % buffer_size; //新しいバッファ位置
				writing_block_ = (size_t)(new_write_pos / buffer_size); //書き込みブロックをセット
				pbump((int)(pbase() - pptr() + new_buf_pos)); //バッファ位置を設定
				return new_write_pos;
			} else {
				pbump((int)off);
				return way + off;
			}
			break;
		case std::ios_base::in:
		{
			pos_type cur_pos = gptr() - eback();
			cur_pos += way + off;
			if (cur_pos < 0 || file_.GetSize() >= cur_pos) {
				return std::char_traits<char>::eof();
			} else {
				setg(eback(), eback() + (int)cur_pos, egptr());
				return cur_pos;
			}
		}
		break;
		default:  //読み書き両用ではエラーを返す
			return std::char_traits<char>::eof();
			break;
		}
	}

	FileStreamBuf::pos_type FileStreamBuf::seekpos(pos_type pos, std::ios_base::openmode mode /*= ios_base::in | ios_base::out*/) {
		return seekoff(pos, std::ios_base::beg, mode);
	}

	bool FileStreamBuf::SaveDataFromWriteBuffer(size_t block, pos_type buf_off) {
		//バッファをファイルに書き込み
		if (file_.WriteData(block*buffer_size, reinterpret_cast<unsigned char*>(pptr()), pptr() - pbase(), true) == false) { return false; }
		//バッファクリア
		memset(write_buffer_.get(), -1, buffer_size);
		//バッファをセット
		setp(write_buffer_.get(), write_buffer_.get() + buffer_size - 1);
		//位置を指定位置に
		pbump((int)(pbase() - pptr() + buf_off));
		return true;
	}
}