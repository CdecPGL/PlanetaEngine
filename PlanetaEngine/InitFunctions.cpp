#include <cassert>

#include "boost/algorithm/string.hpp"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "EngineConfigData.h"
#include "File.h"
#include "INIFileResource.h"
#include "MakeResource.h"
#include "ProgramDefinitionData.h"


namespace {
	std::ofstream LogFileOutPutStream; //ログ出力用ストリーム
	namespace prog_def {
		constexpr char* SCENE_SECTION{ "Scene" };
		constexpr char* COLLISIONGROUP_SECTION{ "CollisionGroup" };
		constexpr char* COLLISIONMATRIX_SECTION{ "CollisionMatrix" };
		constexpr char* STARTUPSCENE_DATA_NAME("StartupScene");
		constexpr char* GROUPS_DATA_NAME("Groups");
	}
}

namespace planeta_engine {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeLogSystem() {
				//開発モードならコンソールウインドウを作成し、そこにログを出力
				if (system_variables::DevelopmentMode) {
					//コンソール出力を有効化
					debug::SystemLog::instance().ValidateConsoleOutPut();
				}
				//ログ出力ファイルを開く
				LogFileOutPutStream.open(system_variables::file_system::LogDirectory + "\\" + system_variables::file_system::LogOutPutFileName, std::ios::out | std::ios::trunc);
				//システムログ出力先にファイル出力を追加
				debug::SystemLog::instance().AddLogOutStream(LogFileOutPutStream);
				if (debug::SystemLog::instance().Initialize()) {
					return{ true, [] {debug::SystemLog::instance().Finalize(); LogFileOutPutStream.flush(); LogFileOutPutStream.close(); } };
				} else {
					assert(false);
					return{ false,[] {} };
				}
			}
			bool LoadEngineConfig(const std::shared_ptr<FileAccessor> sys_dir_accessor) {
				auto ec_file = sys_dir_accessor->LoadFile(system_variables::file_system::EngineConfigFileName);
				if (ec_file == nullptr) {
					PE_LOG_FATAL("エンジン設定ファイル\"", system_variables::file_system::EngineConfigFileName, "\"が開けませんでした。");
					return false;
				}
				if (engine_config::LoadConfigData(std::const_pointer_cast<File>(ec_file)) == false) {
					PE_LOG_FATAL("エンジン設定ファイル\"", system_variables::file_system::EngineConfigFileName, "\"が読み込めませんでした。");
					return false;
				}
				return true;
			}

			bool LoadProgramDefinition(const std::shared_ptr<FileAccessor> sys_dir_accessor, ProgramDefinitionData* out) {
				auto file = sys_dir_accessor->LoadFile(system_variables::file_system::ProgramDefinitionFileName);
				if (file == nullptr || file->GetStatus() != File::FileStatus::Available) {
					PE_LOG_ERROR("プログラム用定義ファイルが開けませんでした。");
					return false;
				}
				auto ini_res = MakeResource<resources::INIFileResource>();
				//FileからINIリソースを作成する
				if (!ini_res->Create(file)) {
					PE_LOG_ERROR("プログラム用定義ファイルを読み込めませんでした。");
					return false;
				}
				//各種設定データを取得する
				//シーン
				auto s_sec = ini_res->GetSection(prog_def::SCENE_SECTION);
				if (s_sec) {
					try {
						auto startup_scene = s_sec->at(prog_def::STARTUPSCENE_DATA_NAME);
						out->startup_scene_id = startup_scene;
					} catch (std::out_of_range&) {
						PE_LOG_ERROR("プログラム用定義ファイルのシーンに関するセクションに、スタートアップシーンが定義されていません。");
						return false;
					}
				} else {
					PE_LOG_ERROR("プログラム用定義ファイルにシーンに関するセクションが存在しません。");
					return false;
				}
				//衝突グループ
				std::vector<std::string> group_list;
				auto cg_sec = ini_res->GetSection(prog_def::COLLISIONGROUP_SECTION);
				if (cg_sec) {
					try {
						auto groupes = cg_sec->at(prog_def::GROUPS_DATA_NAME);
						boost::split(group_list, groupes, boost::is_any_of(","));
						out->collision_group_matrix.AddCollisionGroups(group_list);
					}
					catch (std::out_of_range&) {
						PE_LOG_ERROR("プログラム用定義ファイルの衝突グループに関するセクションに、グループリストが定義されていません。");
						return false;
					}
				} else {
					PE_LOG_ERROR("プログラム用定義ファイルに衝突グループに関するセクションが存在しません。");
					return false;
				}
				//衝突マトリックス
				auto cm_sec = ini_res->GetSection(prog_def::COLLISIONMATRIX_SECTION);
				if (cm_sec) {
					for (auto&& group_name : group_list) {
						try {
							auto colables = cm_sec->at(group_name);
							std::vector<std::string> colable_list;
							boost::split(colable_list, colables, boost::is_any_of(","));
							for (auto&& colable_group : colable_list) {
								out->collision_group_matrix.SetCollisionFlag(group_name, colable_group, true);
							}
						} catch (std::out_of_range&) {
							continue;
						}
					}
				} else {
					PE_LOG_ERROR("プログラム用定義ファイルに衝突マトリックスに関するセクションが存在しません。");
					return false;
				}

				return true;
			}
		}
	}
}
