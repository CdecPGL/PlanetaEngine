#include "InitFunctions.h"
#include "ResourceManager.h"
#include "SystemVariables.h"

#include "RGraph.h"
#include "RSound.h"
#include "RMusic.h"
#include "RCsv.h"
#include "RText.h"
#include "RXml.h"
#include "RJson.h"
#include "RFontDefinition.h"
#include "RPtree.h"
#include "REffect.h"
#include "REffectTexture.h"

namespace planeta {
	namespace private_ {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(ResourceManager& mgr, const std::shared_ptr<FileManipulatorBase>& res_file_manipurator) {
				//ビルドインリソースの設定
				mgr.AddResourceType<RGraph>(system_variables::resource::BuiltinGraphResourceTypeName); //画像
				mgr.AddResourceType<RSound>(system_variables::resource::BuiltinSoundResourceTypeName); //効果音
				mgr.AddResourceType<RMusic>(system_variables::resource::BuiltinMusicResourceTypeName); //音楽
				mgr.AddResourceType<RCsv>(system_variables::resource::BuiltinCSVResourceTypeName); //CSVファイル
				mgr.AddResourceType<RText>(system_variables::resource::BuiltinTextResourceTypeName); //テキストファイル
				mgr.AddResourceType<RXml>(system_variables::resource::BuiltinXMLResourceTypeName); //XMLファイル
				mgr.AddResourceType<RFontDefinition>(system_variables::resource::BuiltinFontDefinitionResourceTypeName); //フォント定義ファイル
				mgr.AddResourceType<RJson>(system_variables::resource::BuiltinJSONResourceTypeName); //JSONファイル
				mgr.AddResourceType<RPtree>(system_variables::resource::BuiltinPtreeResourceTypeName); //Ptree
				mgr.AddResourceType<REffect>(system_variables::resource::BuiltinEffectResourceTypeName); //Effect
				mgr.AddResourceType<REffectTexture>(system_variables::resource::BuiltinEffectTextureResourceTypeName); //EffectTexture
				mgr.SetFileManipulator_(res_file_manipurator); //ファイルアクセサセット
				mgr.SetResourceListFileName_(private_::system_variables::file_system::ResourceListFileName); //リソースリストファイル名セット
				if (mgr.Initialize()) {
					return{ true, [&mgr] {mgr.Finalize(); } };
				} else {
					PE_LOG_FATAL("リソースシステムの初期化に失敗しました。");
					return{ false, [] {} };
				}
			}
		}
	}
}