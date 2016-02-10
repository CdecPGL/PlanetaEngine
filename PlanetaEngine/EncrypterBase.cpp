#include "EncrypterBase.h"
#include "File.h"

namespace planeta_engine {
	namespace encrypters {

		bool EncrypterBase::Encrypt(const file_system::File& src, file_system::File& dst)const
		{
			if (&src == &dst) { return Encrypt(dst); } //�\�[�X�ƃf�X�e�B�l�[�V�����������������瓯���ł��Ăт���
			else { return EncryptCore(src, dst); }
		}

		bool EncrypterBase::Encrypt(file_system::File& src_and_dst)const
		{
			file_system::File dst; //�ꎞ�f�X�e�B�l�[�V����
			if (EncryptCore(src_and_dst, dst)) 
			{
				src_and_dst = std::move(dst); 
				return true;
			} //�ꎞ�f�X�e�B�l�[�V��������͂Ɉړ�����
			else { return false; }
		}

		bool EncrypterBase::Decrypt(file_system::File& src_and_dst)const
		{
			file_system::File dst; //�ꎞ�f�X�e�B�l�[�V����
			if (DecryptCore(src_and_dst, dst))
			{
				src_and_dst = std::move(dst);
				return true;
			} //�ꎞ�f�X�e�B�l�[�V��������͂Ɉړ�����
			else { return false; }
		}

		bool EncrypterBase::Decrypt(const file_system::File& src, file_system::File& dst)const
		{
			if (&src == &dst) { return Decrypt(dst); } //�\�[�X�ƃf�X�e�B�l�[�V�����������������瓯���ł��Ăт���
			else { return DecryptCore(src, dst); }
		}

		bool EncrypterBase::DecryptPartially(const file_system::File& src, size_t pos, size_t size, file_system::File& dst)const
		{
			if (!dst.ChangeSize(size, false)) { return false; }
			if (&src == &dst) { return false; } //�\�[�X�ƃf�X�e�B�l�[�V�����������ł͂����Ȃ��B
			return DecryptPartiallyCore(src, pos, size, reinterpret_cast<char*>(dst.GetTopPointer()), dst.GetSize());
		}

	}
}