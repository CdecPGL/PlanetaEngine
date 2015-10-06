#include "InitEnd.h"
#include "SystemVariables.h"
#include "ResourceManager.h"
#include "GraphResource.h"
#include "SoundResource.h"
#include "MusicResource.h"
#include "CSVResource.h"
#include "TextResource.h"


namespace planeta_engine {
	namespace core{
		namespace init_end {
			bool SetUpResourceManager() {
				using namespace core;
				using namespace core;
				ResourceManager& rm = ResourceManager::instance();
				//ビルトインリソースの設定
				rm.AddResourceType<resources::GraphResource>(system_variables::BuiltinGraphResourceTypeName); //画像
				rm.AddResourceType<resources::SoundResource>(system_variables::BuiltinSoundResourceTypeName); //効果音
				rm.AddResourceType<resources::MusicResource>(system_variables::BuiltinMusicResourceTypeName); //音楽
				rm.AddResourceType<resources::CSVResource>(system_variables::BuiltinCSVResourceTypeName); //CSVファイル
				rm.AddResourceType<resources::TextResource>(system_variables::BuiltinTextResourceTypeName); //テキストファイル
				return true;
			}
		}
	}
}