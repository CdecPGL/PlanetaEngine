#include "file_stream_buf.hpp"
#include "file.hpp"

namespace plnt {
	const size_t file_stream_buf::buffer_size = 256;

	file_stream_buf::file_stream_buf(file &file) : file_(file), write_buffer_(std::make_unique<char[]>(buffer_size)) {
		write_buffer_[0] = static_cast<char>(std::char_traits<char>::eof());
		const auto top = reinterpret_cast<char *>(file.top_pointer());
		setg(top, top, top + file.size());
	}

	file_stream_buf::int_type file_stream_buf::overflow(const int_type d/*= char_traits<char>::eof()*/) {
		save_data_from_write_buffer(writing_block_++, 1);
		//あふれた1バイト分書き込んむ
		write_buffer_[0] = static_cast<char>(d);
		return d;
	}

	file_stream_buf::int_type file_stream_buf::underflow() { return std::char_traits<char>::eof(); }

	file_stream_buf::int_type file_stream_buf::uflow() { return std::char_traits<char>::eof(); }

	int file_stream_buf::sync() {
		//バッファに書き込みがあった分だけコピーし、リセット
		return save_data_from_write_buffer(writing_block_, pptr() - pbase()) ? 0 : -1;
	}

	file_stream_buf::pos_type file_stream_buf::seekoff(const off_type off, const std::ios_base::seekdir dir,
	                                               const std::ios_base::openmode mode
	                                               /*= ios_base::in | ios_base::out*/) {
		size_t buf_pos;
		pos_type way = dir == std::ios_base::beg ? 0 : static_cast<int64_t>(file_.size()) - 1;
		switch (mode & (std::ios_base::in | std::ios_base::out)) {
			case std::ios_base::out:
				//書き込み位置
				buf_pos = pptr() - pbase();
				if (dir == std::ios_base::cur) { way = static_cast<int64_t>(writing_block_ * buffer_size + buf_pos); }
				if (buf_pos + off < 0 || buf_pos + off >= buffer_size) {
					save_data_from_write_buffer(writing_block_, 0); //現在のバッファをファイルに書き込み
					const auto new_write_pos = static_cast<int64_t>(writing_block_ * buffer_size + buf_pos) + off;
					//新しい書き込み位置
					if (new_write_pos < 0) { return std::char_traits<char>::eof(); } //負だったらだめ
					const auto new_buf_pos = new_write_pos % buffer_size; //新しいバッファ位置
					writing_block_ = new_write_pos / buffer_size; //書き込みブロックをセット
					pbump(static_cast<int>(pbase() - pptr() + new_buf_pos)); //バッファ位置を設定
					return new_write_pos;
				}
				pbump(static_cast<int>(off));
				return way + off;
			case std::ios_base::in: {
				pos_type cur_pos = gptr() - eback();
				cur_pos += way + off;
				if (cur_pos < 0 || static_cast<int64_t>(file_.size()) >= cur_pos) {
					return std::char_traits<char>::eof();
				}
				setg(eback(), eback() + static_cast<int>(cur_pos), egptr());
				return cur_pos;
			}
			default: //読み書き両用ではエラーを返す
				return std::char_traits<char>::eof();
		}
	}

	file_stream_buf::pos_type file_stream_buf::seekpos(const pos_type pos,
	                                               const std::ios_base::openmode mode
	                                               /*= ios_base::in | ios_base::out*/) {
		return seekoff(pos, std::ios_base::beg, mode);
	}

	bool file_stream_buf::save_data_from_write_buffer(const size_t block, pos_type buf_off) {
		//バッファをファイルに書き込み
		if (file_.write_data(block * buffer_size, reinterpret_cast<unsigned char *>(pptr()), pptr() - pbase(), true) ==
			false) { return false; }
		//バッファクリア
		memset(write_buffer_.get(), -1, buffer_size);
		//バッファをセット
		setp(write_buffer_.get(), write_buffer_.get() + buffer_size - 1);
		//位置を指定位置に
		pbump(static_cast<int>(pbase() - pptr() + buf_off));
		return true;
	}
}
