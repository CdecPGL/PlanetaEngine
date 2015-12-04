#include "EXOREncrypter.h"
#include "File.h"

/*�f�[�^�\���ɂ���
�擪32bit�ɂ̓t�@�C���T�C�Y���L�^����Ă���B
������܂߂đS�̂�32bit�P�ʂňÍ��������B
*/

namespace {
	constexpr size_t HEADER_BYTE_SIZE(4);
}

namespace planeta_engine {
	namespace encrypters {
		bool EXOREncrypter::DecryptPartiallyCore(const file_system::File& src, size_t pos, size_t size, char* dst_ptr, size_t dst_size) const
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		bool EXOREncrypter::EncryptCore(const file_system::File& src, file_system::File& dst) const
		{
			int b_key = main_key_;
			uint_fast32_t buf;
			size_t block_num = (src.GetSize() + block_byte_size - 1) / block_byte_size; //�K�v�u���b�N��
			//�f�B�X�e�B�l�[�V�����̃T�C�Y�ύX
			dst.ChangeSize(block_num*block_byte_size + HEADER_BYTE_SIZE);
			//�T�C�Y��������
			buf = src.GetSize() ^ b_key;
			dst.WriteData(0, buf);
			//�Í�������������
			for (size_t block = 0; block < block_num; ++block, b_key += sub_key_) {
				src.ReadData(block*block_byte_size, buf);
				buf ^= b_key;
				dst.WriteData(block*block_byte_size + HEADER_BYTE_SIZE, buf, false);
			}
			return true;
		}

		bool EXOREncrypter::DecryptCore(const file_system::File& src, file_system::File& dst) const
		{
			int b_key = main_key_;
			uint_fast32_t buf;
			//�T�C�Y�擾
			src.ReadData(0, buf);
			uint_fast32_t size = buf ^ b_key; 
			//�u���b�N���v�Z
			if ((src.GetSize() - HEADER_BYTE_SIZE) % block_byte_size != 0) { //�u���b�N������Ȃ�
				return false;
			}
			size_t block_num = (src.GetSize() - HEADER_BYTE_SIZE) / block_byte_size;
			//�f�B�X�e�B�l�[�V�����̃T�C�Y�ύX
			dst.ChangeSize(size, false);
			//����������������
			for (size_t block = 0; block < block_num; ++block,b_key += sub_key_) {
				src.ReadData(block*block_byte_size+HEADER_BYTE_SIZE, buf);
				buf ^= b_key;
				if (block == block_num - 1) { //�ŏI�u���b�N��������
					size_t rest = (block_num - 1)*block_byte_size - size; //�c��̃f�[�^�T�C�Y
					dst.WriteData(block*block_byte_size, reinterpret_cast<unsigned char*>(&buf), rest, false);
				}
				else {
					dst.WriteData(block*block_byte_size, buf, false);
				}
			}
			return true;
		}

		uint_fast32_t EXOREncrypter::block_key(size_t block) const
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		void EXOREncrypter::CalculateKey()
		{
			main_key_ = static_cast<uint_fast32_t>(key_); //key�̉�32bit
			sub_key_ = static_cast<uint_fast32_t>(key_ >> 32); //key�̏�32bit
		}

	}
}
