#include <fstream>
#include "NormalFolderSaver.h"
#include "SystemLog.h"
#include "boost/filesystem.hpp"
#include "File.h"
#include "EncrypterBase.h"

namespace planeta_engine {
	namespace file_system {
		bool NormalFileSaver::SaveFile(const std::string& name, const File& file)
		{
			std::ofstream ofs(path() + "\\" + name, std::ios::binary | std::ios::trunc);
			if (!ofs) { 
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��", name, "���f�B���N�g��", path(), "�ɕۑ��ł��܂���ł���");
				return false;
			}
			if (is_encrypter_valid()) { //�Í������L����������
				File encd_file;
				if (!encrypter()->Encrypt(file, encd_file)) { //�Í�������
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�Í����Ɏ��s���܂����B");
					return false;
				}
				ofs.write(reinterpret_cast<char*>(encd_file.GetTopPointer()), encd_file.GetSize()); //�ۑ�
			}
			else {
				ofs.write(reinterpret_cast<const char*>(file.GetTopPointer()), file.GetSize());
			}
			return true;
		}

		bool NormalFileSaver::InitializeCore()
		{
			//�p�X�̑��݊m�F
			boost::filesystem::path rel_path(path());
			if (boost::filesystem::exists(rel_path) && boost::filesystem::is_directory(rel_path)) {
				return true;
			}
			else {
				if (auto_create()) {
					if (boost::filesystem::create_directories(rel_path)) { return true; }
					else { 
						debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������Ɏ��s���܂����B�f�B���N�g��", path(), "�̍쐬�Ɏ��s���܂���");
						return false; 
					}
				}
				else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�������Ɏ��s���܂����B�f�B���N�g��", path(), "�����݂��܂���B");
					return false;
				}
			}
		}

		void NormalFileSaver::FinalizeCore()
		{

		}
	}
}

