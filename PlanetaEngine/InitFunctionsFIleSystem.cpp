#include <cassert>

#include "InitFunctions.h"
#include "FileSystemManager.h"
#include "SystemVariables.h"

#include "FileAccessMode.h"
#include "ArchiveManipulator.h"
#include "NormalFolderManipulator.h"

namespace planeta {
	namespace private_ {
		namespace init_funcs {
			std::shared_ptr<FileAccessor> CreateFileAccessor(FileSystemManager& mgr, FileAccessorKind kind) {
				switch (kind) {
				case FileAccessorKind::Resource: //リソース用ファイルアクセサ設定(速度優先の暗号化)
					return mgr.CreateFileAccessor(system_variables::file_system::ResourceFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::file_system::ResourceDataDirectory + "\\" + system_variables::file_system::DevResourceDataFolderName, false), AccessMode::ReadOnly);
				case FileAccessorKind::SaveData://SaveData用ファイルアクセサ設定(速度優先の暗号化)
					return mgr.CreateFileAccessor(system_variables::file_system::SaveDataFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::file_system::SaveDataDirectory, true), AccessMode::ReadWrite);
				case FileAccessorKind::System://system用ファイルアクセサ設定(高強度な暗号化)
					return mgr.CreateFileAccessor(system_variables::file_system::SystemFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::file_system::SystemDataDirectory, false), AccessMode::ReadOnly);
				case FileAccessorKind::Config://config用ファイルアクセサ設定(暗号化なし)
					return mgr.CreateFileAccessor(system_variables::file_system::ConfigFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::file_system::ConfigFileDirectory, true), AccessMode::ReadWrite);
				default:
					assert(false);
					return nullptr;
				}
			}
		}
	}
}