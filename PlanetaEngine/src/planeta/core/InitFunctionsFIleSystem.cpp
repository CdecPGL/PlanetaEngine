#include <cassert>

#include "InitFunctions.hpp"
#include "SystemVariables.hpp"

#include "file_access_mode.hpp"
#include "archive_manipulator.hpp"
#include "normal_folder_manipulator.hpp"

namespace plnt {
	namespace private_ {
		namespace init_funcs {
			std::shared_ptr<file_manipulator> CreateFileManipurator(FileAccessorKind kind) {
				switch (kind) {
					case FileAccessorKind::Resource: //リソース用ファイルアクセサ設定(速度優先の暗号化)
					{
						auto manipurator = std::make_shared<normal_folder_manipulator>();
						if (manipurator->open(
							system_variables::file_system::ResourceDataDirectory + "\\" +
							system_variables::file_system::DevResourceDataFolderName, access_mode::read_only, false)) {
							return manipurator;
						} else { return nullptr; }
					}
					case FileAccessorKind::SaveData: //SaveData用ファイルアクセサ設定(速度優先の暗号化)
					{
						auto manipurator = std::make_shared<normal_folder_manipulator>();
						if (manipurator->open(system_variables::file_system::SaveDataDirectory, access_mode::read_write,
						                      true)) { return manipurator; } else { return nullptr; }
					}
					case FileAccessorKind::System: //system用ファイルアクセサ設定(高強度な暗号化)
					{
						auto manipurator = std::make_shared<normal_folder_manipulator>();
						if (manipurator->open(system_variables::file_system::SystemDataDirectory, access_mode::read_only,
						                      false)) { return manipurator; } else { return nullptr; }
					}
					case FileAccessorKind::Config: //config用ファイルアクセサ設定(暗号化なし)
					{
						auto manipurator = std::make_shared<normal_folder_manipulator>();
						if (manipurator->open(system_variables::file_system::ConfigFileDirectory, access_mode::read_write,
						                      true)) { return manipurator; } else { return nullptr; }
					}
					default:
						assert(false);
						return nullptr;
				}
			}
		}
	}
}
