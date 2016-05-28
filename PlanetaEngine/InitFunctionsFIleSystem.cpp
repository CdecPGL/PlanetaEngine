#include <cassert>

#include "InitFunctions.h"
#include "FileSystemManager.h"
#include "SystemVariables.h"

#include "FileAccessMode.h"
#include "ArchiveManipulator.h"
#include "NormalFolderManipulator.h"

namespace planeta_engine {
	namespace core {
		namespace init_funcs {
			std::shared_ptr<FileAccessor> CreateFileAccessor(FileAccessorKind kind) {
				FileSystemManager& flm = FileSystemManager::instance();
				switch (kind) {
				case FileAccessorKind::Resource: //���\�[�X�p�t�@�C���A�N�Z�T�ݒ�(���x�D��̈Í���)
					return flm.CreateFileAccessor(system_variables::file_system::ResourceFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::file_system::ResourceDataDirectory + "\\" + system_variables::file_system::DevResourceDataFolderName, false), AccessMode::ReadOnly);
				case FileAccessorKind::SaveData://SaveData�p�t�@�C���A�N�Z�T�ݒ�(���x�D��̈Í���)
					return flm.CreateFileAccessor(system_variables::file_system::SaveDataFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::file_system::SaveDataDirectory, true), AccessMode::ReadWrite);
				case FileAccessorKind::System://system�p�t�@�C���A�N�Z�T�ݒ�(�����x�ȈÍ���)
					return flm.CreateFileAccessor(system_variables::file_system::SystemFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::file_system::SystemDataDirectory, false), AccessMode::ReadOnly);
				case FileAccessorKind::Config://config�p�t�@�C���A�N�Z�T�ݒ�(�Í����Ȃ�)
					return flm.CreateFileAccessor(system_variables::file_system::ConfigFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::file_system::ConfigFileDirectory, true), AccessMode::ReadWrite);
				default:
					assert(false);
					return nullptr;
				}
			}
		}
	}
}