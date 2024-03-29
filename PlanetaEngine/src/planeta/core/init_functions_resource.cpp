﻿#include "init_functions.hpp"
#include "resource_manager.hpp"
#include "system_variables.hpp"

#include "..\buildin\r_graph.hpp"
#include "..\buildin\r_sound.hpp"
#include "..\buildin\r_music.hpp"
#include "..\buildin\r_csv.hpp"
#include "..\buildin\r_font.hpp"
#include "..\buildin\r_text.hpp"
#include "..\buildin\r_xml.hpp"
#include "..\buildin\r_json.hpp"
#include "..\buildin\r_effect.hpp"
#include "..\buildin\r_effect_texture.hpp"
#include "..\buildin\r_game_object.hpp"

namespace plnt::private_::init_funcs {
	std::tuple<bool, std::function<void()>> initialize_resource_system(
		resource_manager &mgr, const std::shared_ptr<file_manipulator> &res_dir_manipulator) {
		//ビルドインリソースの設定
		mgr.add_resource_type<r_graph>(system_variables::resource::builtin_graph_resource_type_name,
		                            system_variables::resource::builtin_graph_resource_type_prefix); //画像
		mgr.add_resource_type<r_sound>(system_variables::resource::builtin_sound_resource_type_name,
		                            system_variables::resource::builtin_sound_resource_type_prefix); //効果音
		mgr.add_resource_type<r_music>(system_variables::resource::builtin_music_resource_type_name,
		                            system_variables::resource::builtin_music_resource_type_prefix); //音楽
		mgr.add_resource_type<r_csv>(system_variables::resource::builtin_csv_resource_type_name,
		                          system_variables::resource::builtin_csv_resource_type_prefix); //CSVファイル
		mgr.add_resource_type<r_text>(system_variables::resource::builtin_text_resource_type_name,
		                           system_variables::resource::builtin_text_resource_type_prefix); //テキストファイル
		mgr.add_resource_type<r_xml>(system_variables::resource::builtin_xml_resource_type_name,
		                          system_variables::resource::builtin_xml_resource_type_prefix); //XMLファイル
		mgr.add_resource_type<r_font>(system_variables::resource::builtin_font_resource_type_name,
		                           system_variables::resource::builtin_font_resource_type_prefix); //フォントファイル
		mgr.add_resource_type<r_json>(system_variables::resource::builtin_json_resource_type_name,
		                           system_variables::resource::builtin_json_resource_type_prefix); //JSONファイル
		mgr.add_resource_type<r_game_object>(system_variables::resource::builtin_game_object_resource_type_name,
		                                 system_variables::resource::builtin_game_object_resource_type_prefix);
		//GameObject
		//mgr.AddResourceType<RGameObjectComponent>(system_variables::resource::BuiltinGameObjectComponentResourceTypeName, system_variables::resource::BuiltinGameObjectComponentResourceTypePrefix); //GameObject
		mgr.add_resource_type<r_effect>(system_variables::resource::builtin_effect_resource_type_name,
		                             system_variables::resource::builtin_effect_resource_type_prefix); //Effect
		mgr.add_resource_type<r_effect_texture>(system_variables::resource::builtin_effect_texture_resource_type_name,
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
