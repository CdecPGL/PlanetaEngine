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

namespace planeta {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(const std::shared_ptr<FileAccessor> res_file_accessor) {
				auto& rm = ResourceManager::instance();
				//ビルドインリソースの設定
				rm.AddResourceType<RGraph>(system_variables::resource::BuiltinGraphResourceTypeName); //画像
				rm.AddResourceType<RSound>(system_variables::resource::BuiltinSoundResourceTypeName); //効果音
				rm.AddResourceType<RMusic>(system_variables::resource::BuiltinMusicResourceTypeName); //音楽
				rm.AddResourceType<RCsv>(system_variables::resource::BuiltinCSVResourceTypeName); //CSVファイル
				rm.AddResourceType<RText>(system_variables::resource::BuiltinTextResourceTypeName); //テキストファイル
				rm.AddResourceType<RXml>(system_variables::resource::BuiltinXMLResourceTypeName); //XMLファイル
				rm.AddResourceType<RFontDefinition>(system_variables::resource::BuiltinFontDefinitionResourceTypeName); //フォント定義ファイル
				rm.AddResourceType<RJson>(system_variables::resource::BuiltinJSONResourceTypeName); //JSONファイル
				rm.SetFileAccessor_(res_file_accessor); //ファイルアクセサセット
				rm.SetResourceListFileName_(core::system_variables::file_system::ResourceListFileName); //リソースリストファイル名セット
				if (rm.Initialize()) {
					return{ true, [] {ResourceManager::instance().Finalize(); } };
				} else {
					PE_LOG_FATAL("リソースシステムの初期化に失敗しました。");
					return{ false, [] {} };
				}
			}
		}
	}
}