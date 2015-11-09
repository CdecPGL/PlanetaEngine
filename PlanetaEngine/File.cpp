#include "File.h"

namespace planeta_engine{
	namespace file_system{

		File::File()
		{
		}

		File::File(const std::string& ext) :extension(ext){}


		File::~File()
		{
			if (data_top){
				delete[] data_top;
			}
		}

		std::string File::GetExtension()const{ return extension; }
		unsigned int File::GetSize()const{ return size_; }
		const unsigned char* File::GetTopPointer()const{ return data_top; }
		unsigned char* File::GetTopPointer(){ return data_top; }
		File::FileStatus File::GetStatus()const{ return status; }

		void File::UnloadData(){
			if (data_top){
				size_ = 0;
				delete[] data_top;
				data_top = nullptr;
				status = FileStatus::Unloaded;
			}
		}

		void File::ErrorOccured(){
			status = FileStatus::Error;
		}

		bool File::ChangeSize(size_t size,bool copy)
		{
			unsigned char* new_area = new unsigned char[size];
			if (new_area == nullptr) { ErrorOccured(); false; } //�������̊m�ۂɎ��s����
			if (data_top) { //���łɃ��������m�ۂ���Ă�����
				if (copy) { memcpy_s(new_area, size, data_top, size_); }
				delete[] data_top;
			}
			data_top = new_area;
			status = FileStatus::Available;
			return true;
		}

		bool File::WriteData(size_t pos, unsigned char* data_top, size_t data_size, bool auto_extend /*= false*/)
		{
			//�f�[�^�T�C�Y���t�@�C���T�C�Y���傫��������
			if (GetSize() < pos + data_size) { //�����g�����L����������T�C�Y�𑝂₷
				if (auto_extend) { ChangeSize(pos + data_size, true); }
				else { return false; } //������������G���[
			}
			memcpy_s(GetTopPointer() + pos, size_ - pos, data_top, data_size);
			return true;
		}

		bool File::ReadData(size_t pos, unsigned char* buffer_top, size_t buffer_size) const
		{
			if (GetSize() < pos + buffer_size) {
				//�o�b�t�@�����t�@�C���T�C�Y���傫��������A�t�@�C���I�[�܂ł��o�b�t�@�ɃR�s�[
				memcpy_s(buffer_top, buffer_size, GetTopPointer() + pos, GetSize() - pos);
			}
			else {
				//�o�b�t�@�����Z��������o�b�t�@��t�ɃR�s�[
				memcpy_s(buffer_top, buffer_size, GetTopPointer() + pos, buffer_size);
			}
			return true;
		}

		void File::SetData(unsigned char* top_ptr, size_t size)
		{
			if (data_top) { free(data_top); }
			data_top = top_ptr;
			size_ = size;
			status = FileStatus::Available;
		}

	}
}
