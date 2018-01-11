#include "planeta/EncrypterBase.hpp"
#include "planeta/core/File.hpp"

namespace plnt {
	namespace encrypters {

		bool EncrypterBase::Encrypt(const File& src, File& dst)const
		{
			if (&src == &dst) { return Encrypt(dst); } //ソースとデスティネーションが同じだったら同じ版を呼びだす
			else { return EncryptCore(src, dst); }
		}

		bool EncrypterBase::Encrypt(File& src_and_dst)const
		{
			File dst; //一時デスティネーション
			if (EncryptCore(src_and_dst, dst)) 
			{
				src_and_dst = std::move(dst); 
				return true;
			} //一時デスティネーションを入力に移動する
			else { return false; }
		}

		bool EncrypterBase::Decrypt(File& src_and_dst)const
		{
			File dst; //一時デスティネーション
			if (DecryptCore(src_and_dst, dst))
			{
				src_and_dst = std::move(dst);
				return true;
			} //一時デスティネーションを入力に移動する
			else { return false; }
		}

		bool EncrypterBase::Decrypt(const File& src, File& dst)const
		{
			if (&src == &dst) { return Decrypt(dst); } //ソースとデスティネーションが同じだったら同じ版を呼びだす
			else { return DecryptCore(src, dst); }
		}

		bool EncrypterBase::DecryptPartially(const File& src, size_t pos, size_t size, File& dst)const
		{
			if (!dst.Reserve(size, false)) { return false; }
			if (&src == &dst) { return false; } //ソースとデスティネーションが同じではいけない。
			return DecryptPartiallyCore(src, pos, size, reinterpret_cast<char*>(dst.top_pointer()), dst.size());
		}

	}
}