#include "init_functions.hpp"
#include "ResourceManager.hpp"
#include "SystemVariables.hpp"

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
		mgr.add_resource_type<RGraph>(system_variables::resource::BuiltinGraphResourceTypeName,
		                            system_variables::resource::BuiltinGraphResourceTypePrefix); //画像
		mgr.add_resource_type<RSound>(system_variables::resource::BuiltinSoundResourceTypeName,
		                            system_variables::resource::BuiltinSoundResourceTypePrefix); //効果音
		mgr.add_resource_type<RMusic>(system_variables::resource::BuiltinMusicResourceTypeName,
		                            system_variables::resource::BuiltinMusicResourceTypePrefix); //音楽
		mgr.add_resource_type<RCsv>(system_variables::resource::BuiltinCSVResourceTypeName,
		                          system_variables::resource::BuiltinCSVResourceTypePrefix); //CSVファイル
		mgr.add_resource_type<RText>(system_variables::resource::BuiltinTextResourceTypeName,
		                           system_variables::resource::BuiltinTextResourceTypePrefix); //テキストファイル
		mgr.add_resource_type<RXml>(system_variables::resource::BuiltinXMLResourceTypeName,
		                          system_variables::resource::BuiltinXMLResourceTypePrefix); //XMLファイル
		mgr.add_resource_type<RFont>(system_variables::resource::BuiltinFontResourceTypeName,
		                           system_variables::resource::BuiltinFontResourceTypePrefix); //フォントファイル
		mgr.add_resource_type<RJson>(system_variables::resource::BuiltinJSONResourceTypeName,
		                           system_variables::resource::BuiltinJSONResourceTypePrefix); //JSONファイル
		mgr.add_resource_type<RGameObject>(system_variables::resource::BuiltinGameObjectResourceTypeName,
		                                 system_variables::resource::BuiltinGameObjectResourceTypePrefix);
		//GameObject
		//mgr.AddResourceType<RGameObjectComponent>(system_variables::resource::BuiltinGameObjectComponentResourceTypeName, system_variables::resource::BuiltinGameObjectComponentResourceTypePrefix); //GameObject
		mgr.add_resource_type<REffect>(system_variables::resource::BuiltinEffectResourceTypeName,
		                             system_variables::resource::BuiltinEffectResourceTypePrefix); //Effect
		mgr.add_resource_type<REffectTexture>(system_variables::resource::BuiltinEffectTextureResourceTypeName,
		                                    system_variables::resource::BuiltinEffectTextureResourceTypePrefix);
		//EffectTexture
		mgr.set_file_manipulator(res_dir_manipulator); //ファイルアクセサセット
		mgr.set_resource_list_file_name(system_variables::file_system::ResourceListFileName);
		//リソースリストファイル名セット
		if (mgr.initialize()) { return {true, [&mgr] { mgr.finalize(); }}; }
		PE_LOG_FATAL("リソースシステムの初期化に失敗しました。");
		return {
			false, [] { }
		};
	}
}
