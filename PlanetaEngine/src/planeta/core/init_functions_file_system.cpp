#include <cassert>

#include "init_functions.hpp"
#include "SystemVariables.hpp"
#include "file_access_mode.hpp"
#include "archive_manipulator.hpp"
#include "normal_folder_manipulator.hpp"

namespace plnt::private_::init_funcs {
	std::shared_ptr<file_manipulator> create_file_manipulator(const file_accessor_kind kind) {
		switch (kind) {
			case file_accessor_kind::resource: //リソース用ファイルアクセサ設定(速度優先の暗号化)
			{
				if (auto manipulator = std::make_shared<normal_folder_manipulator>(); manipulator->open(
					system_variables::file_system::ResourceDataDirectory + "\\" +
					system_variables::file_system::DevResourceDataFolderName, access_mode::read_only, false)) {
					return manipulator;
				}
				return nullptr;
			}
			case file_accessor_kind::save_data: //SaveData用ファイルアクセサ設定(速度優先の暗号化)
			{
				if (auto manipulator = std::make_shared<normal_folder_manipulator>(); manipulator->open(
					system_variables::file_system::SaveDataDirectory, access_mode::read_write,
					true)) { return manipulator; }
				return nullptr;
			}
			case file_accessor_kind::system: //system用ファイルアクセサ設定(高強度な暗号化)
			{
				if (auto manipulator = std::make_shared<normal_folder_manipulator>(); manipulator->open(
					system_variables::file_system::SystemDataDirectory, access_mode::read_only,
					false)) { return manipulator; }
				return nullptr;
			}
			case file_accessor_kind::config: //config用ファイルアクセサ設定(暗号化なし)
			{
				if (auto manipulator = std::make_shared<normal_folder_manipulator>(); manipulator->open(
					system_variables::file_system::ConfigFileDirectory, access_mode::read_write,
					true)) { return manipulator; }
				return nullptr;
			}
		}

		assert(false);
		return nullptr;
	}
}
