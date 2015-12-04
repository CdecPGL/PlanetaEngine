#include "FileStreamBuf.h"
#include "File.h"

namespace planeta_engine {
	namespace file_system {
		const size_t FileStreamBuf::buffer_size = 256;

		FileStreamBuf::FileStreamBuf(File& file):file_(file),write_buffer_(std::make_unique<char[]>(buffer_size)), read_buffer_(std::make_unique<char[]>(buffer_size))
		{
			write_buffer_[0] = (char)std::char_traits<char>::eof();
			read_buffer_[0] = (char)std::char_traits<char>::eof();
		}

		FileStreamBuf::int_type FileStreamBuf::overflow(int_type d/*= char_traits<char>::eof()*/)
		{
			SaveDataFromWriteBuffer(writing_block_++, 1);
			//あふれた1バイト分書き込んむ
			write_buffer_[0] = (char)d;
			return d;
		}

		FileStreamBuf::int_type FileStreamBuf::underflow()
		{
			LoadDataToReadBuffer(--reading_block_, buffer_size - 1);
			return read_buffer_[buffer_size - 1];
		}

		FileStreamBuf::int_type FileStreamBuf::uflow()
		{
			gbump(gptr() - eback());
			/*LoadDataToReadBuffer(--reading_block_);
			//一つ進めておく
			gbump(1);*/
			return read_buffer_[0];
		}

		int FileStreamBuf::sync()
		{
			//バッファに書き込みがあった分だけコピーし、リセット
			return SaveDataFromWriteBuffer(writing_block_, pptr() - pbase()) ? 0 : -1;
		}

		FileStreamBuf::pos_type FileStreamBuf::seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode mode/*= ios_base::in | ios_base::out*/)
		{
			size_t buf_pos;
			pos_type way = dir == std::ios_base::beg ? 0 : file_.GetSize() - 1;
			switch (mode & (std::ios_base::in | std::ios_base::out))
			{
			case std::ios_base::out:
				//書き込み位置
				buf_pos = pptr() - pbase();
				if (dir == std::ios_base::cur) { way = writing_block_*buffer_size + buf_pos; }
				if (buf_pos + off < 0 || buf_pos + off >= buffer_size) {
					SaveDataFromWriteBuffer(writing_block_,0); //現在のバッファをファイルに書き込み
					auto new_write_pos = (int64_t)writing_block_ * buffer_size + buf_pos + off; //新しい書き込み位置
					if (new_write_pos < 0) { return std::ios::off_type(-1); } //負だったらだめ
					auto new_buf_pos = new_write_pos % buffer_size; //新しいバッファ位置
					writing_block_ = (size_t)(new_write_pos / buffer_size); //書き込みブロックをセット
					pbump((int)(pbase() - pptr() + new_buf_pos)); //バッファ位置を設定
					return new_write_pos;
				}
				else {
					pbump((int)off);
					return way + off;
				}
				break;
			case std::ios_base::in:
				//読み込み位置
				buf_pos = gptr() - eback();
				if (dir == std::ios_base::cur) { way = reading_block_*buffer_size + buf_pos; }
				if (buf_pos + off < 0 || buf_pos + off >= buffer_size) {
					auto new_read_pos = (int)reading_block_ * buffer_size + buf_pos + off; //新しい読み込み位置
					if (new_read_pos < 0) { return std::ios::off_type(-1); } //負だったらエラー
					else if(new_read_pos >= file_.GetSize()){ return std::ios::off_type(-1); } //ファイルサイズを超えていてもエラー
					auto new_buf_pos = new_read_pos % buffer_size; //新しいバッファ位置
					reading_block_ = (size_t)(new_read_pos / buffer_size); //読み込みブロックをセット
					LoadDataToReadBuffer(reading_block_, new_buf_pos); //バッファに読み込み
					return new_read_pos;
				}
				else {
					gbump((int)off);
					return way + off;
				}
				break;
			default:  //読み書き両用ではエラーを返す
				return std::ios::off_type(-1);
				break;
			}
		}

		FileStreamBuf::int_type FileStreamBuf::pbackfail(int_type /*= char_traits<char>::eof()*/)
		{
			if (gptr() == eback()) {
				LoadDataToReadBuffer(--reading_block_, buffer_size - 1);
				return read_buffer_[buffer_size - 1];
			}
			else {
				gbump(-1);
				return read_buffer_[gptr() - eback()];
			}
		}

		FileStreamBuf::pos_type FileStreamBuf::seekpos(pos_type pos, std::ios_base::openmode mode /*= ios_base::in | ios_base::out*/)
		{
			return seekoff(pos, std::ios_base::beg, mode);
		}

		bool FileStreamBuf::LoadDataToReadBuffer(size_t block, pos_type buf_off)
		{
			//ファイルの先頭位置
			size_t pos = block * buffer_size;
			//バッファ一杯に読み込めない場合は読み込むサイズを調整する。
			size_t read_size = pos + buffer_size >= file_.GetSize() ? file_.GetSize() - pos : buffer_size;
			if (file_.ReadData(pos, reinterpret_cast<unsigned char*>(gptr()), read_size) == false) { return false; }
			//必要なら終端にeof付加(ファイルの終端に達した場合)
			if (read_size < buffer_size) {
				read_buffer_[buffer_size - 1] = (char)std::char_traits<char>::eof();
			}
			//バッファセットして指定位置に
			setg(read_buffer_.get(), read_buffer_.get()+(int)buf_off, read_buffer_.get() + buffer_size - 1);
			return true;
		}

		bool FileStreamBuf::SaveDataFromWriteBuffer(size_t block, pos_type buf_off)
		{
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
}