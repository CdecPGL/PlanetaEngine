#include <cassert>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "FileAccessor.h"
#include "EngineConfigData.h"
#include "File.h"
#include "JsonFile.h"
#include "ProgramDefinitionData.h"


namespace {
	namespace prog_def {
		constexpr char* SCENE_SECTION{ "Scene" };
		constexpr char* COLLISION_SECTION{ "Collision" };
		constexpr char* COLLISIONGROUP_SECTION{ "Groups" };
		constexpr char* COLLISIONMATRIX_SECTION{ "CollidableMatrix" };
		constexpr char* STARTUPSCENE_DATA_NAME("Startup");
	}
}

namespace planeta {
	namespace private_ {
		namespace init_funcs {
			bool LoadEngineConfig(const std::shared_ptr<FileAccessor> sys_dir_accessor) {
				auto ec_file = sys_dir_accessor->LoadFile(system_variables::file_system::EngineConfigFileName);
				if (ec_file == nullptr) {
					PE_LOG_FATAL("エンジン設定ファイル\"", system_variables::file_system::EngineConfigFileName, "\"が開けませんでした。");
					return false;
				}
				if (engine_config::private_::LoadConfigData(std::const_pointer_cast<File>(ec_file)) == false) {
					PE_LOG_FATAL("エンジン設定ファイル\"", system_variables::file_system::EngineConfigFileName, "\"が読み込めませんでした。");
					return false;
				}
				return true;
			}

			bool LoadProgramDefinition(const std::shared_ptr<FileAccessor> sys_dir_accessor, ProgramDefinitionData* out) {
				auto file = sys_dir_accessor->LoadFile(system_variables::file_system::ProgramDefinitionFileName);
				if (file == nullptr || file->is_available() == false) {
					PE_LOG_ERROR("プログラム用定義ファイルが開けませんでした。");
					return false;
				}
				JsonFile json_file{};
				//FileからJsonリソースを作成する
				if (!json_file.Load(*file)) {
					PE_LOG_ERROR("プログラム用定義ファイルを読み込めませんでした。");
					return false;
				}
				//各種設定データを取得する
				try {
					auto root_obj = json_file.GetRoot().Get<JSONObject>();
					auto scene_obj = root_obj->AtWithException(prog_def::SCENE_SECTION)->GetWithException<JSONObject>();
					auto col_obj = root_obj->AtWithException(prog_def::COLLISION_SECTION)->GetWithException<JSONObject>();
					//シーン
					out->startup_scene_id = *scene_obj->AtWithException(prog_def::STARTUPSCENE_DATA_NAME)->GetWithException<std::string>();
					//衝突システム
					//衝突グループ
					std::vector<std::string> group_list;
					group_list = *col_obj->AtWithException(prog_def::COLLISIONGROUP_SECTION)->GetWithException<std::vector<std::string>>();
					out->collision_group_matrix.AddCollisionGroups(group_list);
					//衝突可能マトリックス
					auto col_mtx = *col_obj->AtWithException(prog_def::COLLISIONMATRIX_SECTION)->GetWithException<std::unordered_map<std::string, std::vector<std::string>>>();
					for (auto&& group : col_mtx) {
						for (auto&& cbl_group : group.second) {
							if (!out->collision_group_matrix.SetCollisionFlag(group.first, cbl_group, true)) {
								PE_LOG_WARNING("衝突グループ\"", group.first, "\"と\"", cbl_group, "\"のフラグの設定に失敗しました。どちらかのグループが定義されていない可能性があります。");
							}
						}
					}
				} catch (std::out_of_range& e) {
					PE_LOG_ERROR("プログラム用定義の取得に失敗しました。内容が不足している可能性があります。(", e.what(), ")");
					return false;
				} catch (JSONTypeError& e) {
					PE_LOG_ERROR("プログラム用定義の取得に失敗しました。型が間違えている可能性があります。(", e.what(), ")");
					return false;
				}
				return true;
			}
		}
	}
}
