#include "InitFunctions.h"
#include "ResourceManager.h"
#include "SystemVariables.h"

#include "GraphResource.h"
#include "SoundResource.h"
#include "MusicResource.h"
#include "CSVResource.h"
#include "TextResource.h"
#include "XMLResource.h"
#include "FontDefinitionResource.h"

namespace planeta_engine {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(const std::shared_ptr<FileAccessor> res_file_accessor) {
				auto& rm = ResourceManager::instance();
				//ビルドインリソースの設定
				rm.AddResourceType<resources::GraphResource>(system_variables::resource::BuiltinGraphResourceTypeName); //画像
				rm.AddResourceType<resources::SoundResource>(system_variables::resource::BuiltinSoundResourceTypeName); //効果音
				rm.AddResourceType<resources::MusicResource>(system_variables::resource::BuiltinMusicResourceTypeName); //音楽
				rm.AddResourceType<resources::CSVResource>(system_variables::resource::BuiltinCSVResourceTypeName); //CSVファイル
				rm.AddResourceType<resources::TextResource>(system_variables::resource::BuiltinTextResourceTypeName); //テキストファイル
				rm.AddResourceType<resources::XMLResource>(system_variables::resource::BuiltinXMLResourceTypeName); //XMLファイル
				rm.AddResourceType<resources::FontDefinitionResource>(system_variables::resource::BuiltinFontDefinitionResourceTypeName); //フォント定義ファイル
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