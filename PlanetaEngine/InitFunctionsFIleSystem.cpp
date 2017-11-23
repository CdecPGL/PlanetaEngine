#include <cassert>

#include "InitFunctions.h"
#include "SystemVariables.h"

#include "FileAccessMode.h"
#include "ArchiveManipulator.h"
#include "NormalFolderManipulator.h"

namespace planeta {
	namespace private_ {
		namespace init_funcs {
			std::shared_ptr<FileManipulator> CreateFileManipurator(FileAccessorKind kind) {
				switch (kind) {
				case FileAccessorKind::Resource: //リソース用ファイルアクセサ設定(速度優先の暗号化)
				{
					auto manipurator = std::make_shared<NormalFolderManipulator>();
					if (manipurator->Open(system_variables::file_system::ResourceDataDirectory + "\\" + system_variables::file_system::DevResourceDataFolderName, AccessMode::ReadOnly, false)) {
						return manipurator;
					}
					else {
						return nullptr;
					}
				}
				case FileAccessorKind::SaveData://SaveData用ファイルアクセサ設定(速度優先の暗号化)
				{
					auto manipurator = std::make_shared<NormalFolderManipulator>();
					if (manipurator->Open(system_variables::file_system::SaveDataDirectory, AccessMode::ReadWrite, true)) {
						return manipurator;
					}
					else {
						return nullptr;
					}
				}
				case FileAccessorKind::System://system用ファイルアクセサ設定(高強度な暗号化)
				{
					auto manipurator = std::make_shared<NormalFolderManipulator>();
					if (manipurator->Open(system_variables::file_system::SystemDataDirectory, AccessMode::ReadOnly, false)) {
						return manipurator;
					}
					else {
						return nullptr;
					}
				}
				case FileAccessorKind::Config://config用ファイルアクセサ設定(暗号化なし)
				{
					auto manipurator = std::make_shared<NormalFolderManipulator>();
					if (manipurator->Open(system_variables::file_system::ConfigFileDirectory, AccessMode::ReadWrite, true)) {
						return manipurator;
					}
					else {
						return nullptr;
					}
				}
				default:
					assert(false);
					return nullptr;
				}
			}
		}
	}
}