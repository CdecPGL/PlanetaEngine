#include "init_functions.hpp"
#include "resource_manager.hpp"
#include "system_variables.hpp"

#include "planeta/buildin/RGraph.hpp"
#include "planeta/buildin/RSound.hpp"
#include "planeta/buildin/RMusic.hpp"
#include "planeta/buildin/RCsv.hpp"
#include "planeta/buildin/RFont.hpp"
#include "planeta/buildin/RText.hpp"
#include "planeta/buildin/RXml.hpp"
#include "planeta/buildin/RJson.hpp"
#include "planeta/buildin/REffect.hpp"
#include "planeta/buildin/REffectTexture.hpp"
#include "planeta/buildin/RGameObject.hpp"

namespace plnt::private_::init_funcs {
	std::tuple<bool, std::function<void()>> initialize_resource_system(
		resource_manager &mgr, const std::shared_ptr<file_manipulator> &res_dir_manipulator) {
		//ビルドインリソースの設定
		mgr.add_resource_type<RGraph>(system_variables::resource::builtin_graph_resource_type_name,
		                            system_variables::resource::builtin_graph_resource_type_prefix); //画像
		mgr.add_resource_type<RSound>(system_variables::resource::builtin_sound_resource_type_name,
		                            system_variables::resource::builtin_sound_resource_type_prefix); //効果音
		mgr.add_resource_type<RMusic>(system_variables::resource::builtin_music_resource_type_name,
		                            system_variables::resource::builtin_music_resource_type_prefix); //音楽
		mgr.add_resource_type<RCsv>(system_variables::resource::builtin_csv_resource_type_name,
		                          system_variables::resource::builtin_csv_resource_type_prefix); //CSVファイル
		mgr.add_resource_type<RText>(system_variables::resource::builtin_text_resource_type_name,
		                           system_variables::resource::builtin_text_resource_type_prefix); //テキストファイル
		mgr.add_resource_type<RXml>(system_variables::resource::builtin_xml_resource_type_name,
		                          system_variables::resource::builtin_xml_resource_type_prefix); //XMLファイル
		mgr.add_resource_type<RFont>(system_variables::resource::builtin_font_resource_type_name,
		                           system_variables::resource::builtin_font_resource_type_prefix); //フォントファイル
		mgr.add_resource_type<RJson>(system_variables::resource::builtin_json_resource_type_name,
		                           system_variables::resource::builtin_json_resource_type_prefix); //JSONファイル
		mgr.add_resource_type<RGameObject>(system_variables::resource::builtin_game_object_resource_type_name,
		                                 system_variables::resource::builtin_game_object_resource_type_prefix);
		//GameObject
		//mgr.AddResourceType<RGameObjectComponent>(system_variables::resource::BuiltinGameObjectComponentResourceTypeName, system_variables::resource::BuiltinGameObjectComponentResourceTypePrefix); //GameObject
		mgr.add_resource_type<REffect>(system_variables::resource::builtin_effect_resource_type_name,
		                             system_variables::resource::builtin_effect_resource_type_prefix); //Effect
		mgr.add_resource_type<REffectTexture>(system_variables::resource::builtin_effect_texture_resource_type_name,
		                                    system_variables::resource::builtin_effect_texture_resource_type_prefix);
		//EffectTexture
		mgr.set_file_manipulator(res_dir_manipulator); //ファイルアクセサセット
		mgr.set_resource_list_file_name(system_variables::file_system::resource_list_file_name);
		//リソースリストファイル名セット
		if (mgr.initialize()) { return {true, [&mgr] { mgr.finalize(); }}; }
		PE_LOG_FATAL("リソースシステムの初期化に失敗しました。");
		return {
			false, [] { }
		};
	}
}
