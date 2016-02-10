#include "EncrypterBase.h"
#include "File.h"

namespace planeta_engine {
	namespace encrypters {

		bool EncrypterBase::Encrypt(const file_system::File& src, file_system::File& dst)const
		{
			if (&src == &dst) { return Encrypt(dst); } //ソースとデスティネーションが同じだったら同じ版を呼びだす
			else { return EncryptCore(src, dst); }
		}

		bool EncrypterBase::Encrypt(file_system::File& src_and_dst)const
		{
			file_system::File dst; //一時デスティネーション
			if (EncryptCore(src_and_dst, dst)) 
			{
				src_and_dst = std::move(dst); 
				return true;
			} //一時デスティネーションを入力に移動する
			else { return false; }
		}

		bool EncrypterBase::Decrypt(file_system::File& src_and_dst)const
		{
			file_system::File dst; //一時デスティネーション
			if (DecryptCore(src_and_dst, dst))
			{
				src_and_dst = std::move(dst);
				return true;
			} //一時デスティネーションを入力に移動する
			else { return false; }
		}

		bool EncrypterBase::Decrypt(const file_system::File& src, file_system::File& dst)const
		{
			if (&src == &dst) { return Decrypt(dst); } //ソースとデスティネーションが同じだったら同じ版を呼びだす
			else { return DecryptCore(src, dst); }
		}

		bool EncrypterBase::DecryptPartially(const file_system::File& src, size_t pos, size_t size, file_system::File& dst)const
		{
			if (!dst.ChangeSize(size, false)) { return false; }
			if (&src == &dst) { return false; } //ソースとデスティネーションが同じではいけない。
			return DecryptPartiallyCore(src, pos, size, reinterpret_cast<char*>(dst.GetTopPointer()), dst.GetSize());
		}

	}
}