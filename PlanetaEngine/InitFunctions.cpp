#include <cassert>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "EngineConfigData.h"
#include "File.h"
#include "JSONResource.h"
#include "MakeResource.h"
#include "ProgramDefinitionData.h"


namespace {
	std::ofstream LogFileOutPutStream; //ログ出力用ストリーム
	namespace prog_def {
		constexpr char* SCENE_SECTION{ "Scene" };
		constexpr char* COLLISION_SECTION{ "Collision" };
		constexpr char* COLLISIONGROUP_SECTION{ "Groups" };
		constexpr char* COLLISIONMATRIX_SECTION{ "CollidableMatrix" };
		constexpr char* STARTUPSCENE_DATA_NAME("Startup");
	}
}

namespace planeta {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeLogSystem() {
				//開発モードならコンソールウインドウを作成し、そこにログを出力
				if (system_variables::DevelopmentMode) {
					//コンソール出力を有効化
					debug::SystemLog::instance().ValidateConsoleOutPut();
				}
				//ログディレクトリがなかったら作る
				if(!boost::filesystem::exists(system_variables::file_system::LogDirectory)) {
					boost::filesystem::create_directory(system_variables::file_system::LogDirectory);
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
				using namespace resources;
				auto file = sys_dir_accessor->LoadFile(system_variables::file_system::ProgramDefinitionFileName);
				if (file == nullptr || file->GetStatus() != File::FileStatus::Available) {
					PE_LOG_ERROR("プログラム用定義ファイルが開けませんでした。");
					return false;
				}
				auto json_res = MakeResource<resources::JSONResource>();
				//FileからINIリソースを作成する
				if (!json_res->Create(file)) {
					PE_LOG_ERROR("プログラム用定義ファイルを読み込めませんでした。");
					return false;
				}
				//各種設定データを取得する
				//シーン
				auto root_obj = json_res->GetRoot().Get<JSONObject>();
				if (!root_obj) {
					PE_LOG_ERROR("プログラム用定義ファイルにルートオブジェクトが存在しません。");
					return false;
				}
				auto s_obj = root_obj->At<JSONObject>(prog_def::SCENE_SECTION);
				if (s_obj) {
					auto startup_scene = s_obj->At<std::string>(prog_def::STARTUPSCENE_DATA_NAME);
					if (startup_scene) {
						out->startup_scene_id = *startup_scene;
					} else {
						PE_LOG_ERROR("プログラム用定義ファイルのシーンに関するオブジェクトに、文字列型の値スタートアップシーンが定義されていません。");
						return false;
					}
				} else {
					PE_LOG_ERROR("プログラム用定義ファイルにシーンに関するオブジェクトが存在しません。");
					return false;
				}
				//衝突システム
				std::vector<std::string> group_list;
				auto c_obj = root_obj->At<JSONObject>(prog_def::COLLISION_SECTION);
				if (c_obj) {
					//衝突グループリスト
					auto g_ary = c_obj->At<JSONArray>(prog_def::COLLISIONGROUP_SECTION);
					if (g_ary) {
						for (size_t i = 0; i < g_ary->size(); ++i) {
							auto group_name = g_ary->At<std::string>(i);
							if (group_name) {
								group_list.push_back(*group_name);
							} else {
								PE_LOG_ERROR("衝突グループリスト", i, "番目の型が不正です。");
								return false;
							}
						}
						out->collision_group_matrix.AddCollisionGroups(group_list);
					} else {
						PE_LOG_ERROR("プログラム用定義ファイルにコリジョンシステムに関するオブジェクトに、配列型の値衝突グループが定義されていません。");
						return false;
					}
					//衝突可能マトリックス
					auto cm_obj = c_obj->At<JSONObject>(prog_def::COLLISIONMATRIX_SECTION);
					if (cm_obj) {
						for (auto&& group_name : group_list) {
							auto cgl = cm_obj->At<JSONArray>(group_name);
							if (cgl) { //ない場合もあるのでなくてもエラーは出さない。
								std::vector<std::string> colable_list;
								for (size_t i = 0; i < cgl->size(); ++i) {
									auto cgn = cgl->At<std::string>(i);
									if (cgn) {
										colable_list.push_back(*cgn);
									} else {
										//エラーにはしない。
										PE_LOG_ERROR("衝突グループ\"", group_name, "\"の衝突可能配列において", i, "番目の要素の型が不正です。");
									}
								}
								for (auto&& colable_group : colable_list) {
									out->collision_group_matrix.SetCollisionFlag(group_name, colable_group, true);
								}
							}
						}
					} else {
						PE_LOG_ERROR("プログラム用定義ファイルにコリジョンシステムに関するオブジェクトに、衝突可能マトリックスオブジェクトが定義されていません。");
						return false;
					}
				} else {
					PE_LOG_ERROR("プログラム用定義ファイルにコリジョンシステムに関するオブジェクトが存在しません。");
					return false;
				}
				return true;
			}
		}
	}
}
