#include "InitFunctions.hpp"
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
#include "planeta/buildin/RPtree.hpp"
#include "planeta/buildin/REffect.hpp"
#include "planeta/buildin/REffectTexture.hpp"
#include "planeta/buildin/RGameObject.hpp"

namespace plnt {
	namespace private_ {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(
				ResourceManager &mgr, const std::shared_ptr<FileManipulator> &res_file_manipurator) {
				//ビルドインリソースの設定
				mgr.AddResourceType<RGraph>(system_variables::resource::BuiltinGraphResourceTypeName,
				                            system_variables::resource::BuiltinGraphResourceTypePrefix); //画像
				mgr.AddResourceType<RSound>(system_variables::resource::BuiltinSoundResourceTypeName,
				                            system_variables::resource::BuiltinSoundResourceTypePrefix); //効果音
				mgr.AddResourceType<RMusic>(system_variables::resource::BuiltinMusicResourceTypeName,
				                            system_variables::resource::BuiltinMusicResourceTypePrefix); //音楽
				mgr.AddResourceType<RCsv>(system_variables::resource::BuiltinCSVResourceTypeName,
				                          system_variables::resource::BuiltinCSVResourceTypePrefix); //CSVファイル
				mgr.AddResourceType<RText>(system_variables::resource::BuiltinTextResourceTypeName,
				                           system_variables::resource::BuiltinTextResourceTypePrefix); //テキストファイル
				mgr.AddResourceType<RXml>(system_variables::resource::BuiltinXMLResourceTypeName,
				                          system_variables::resource::BuiltinXMLResourceTypePrefix); //XMLファイル
				mgr.AddResourceType<RFont>(system_variables::resource::BuiltinFontResourceTypeName,
				                           system_variables::resource::BuiltinFontResourceTypePrefix); //フォントファイル
				mgr.AddResourceType<RJson>(system_variables::resource::BuiltinJSONResourceTypeName,
				                           system_variables::resource::BuiltinJSONResourceTypePrefix); //JSONファイル
				mgr.AddResourceType<RGameObject>(system_variables::resource::BuiltinGameObjectResourceTypeName,
				                                 system_variables::resource::BuiltinGameObjectResourceTypePrefix);
				//GameObject
				//mgr.AddResourceType<RGameObjectComponent>(system_variables::resource::BuiltinGameObjectComponentResourceTypeName, system_variables::resource::BuiltinGameObjectComponentResourceTypePrefix); //GameObject
				mgr.AddResourceType<REffect>(system_variables::resource::BuiltinEffectResourceTypeName,
				                             system_variables::resource::BuiltinEffectResourceTypePrefix); //Effect
				mgr.AddResourceType<REffectTexture>(system_variables::resource::BuiltinEffectTextureResourceTypeName,
				                                    system_variables::resource::BuiltinEffectTextureResourceTypePrefix);
				//EffectTexture
				mgr.SetFileManipulator_(res_file_manipurator); //ファイルアクセサセット
				mgr.SetResourceListFileName_(private_::system_variables::file_system::ResourceListFileName);
				//リソースリストファイル名セット
				if (mgr.Initialize()) { return {true, [&mgr] { mgr.Finalize(); }}; } else {
					PE_LOG_FATAL("リソースシステムの初期化に失敗しました。");
					return {
						false, [] { }
					};
				}
			}
		}
	}
}
