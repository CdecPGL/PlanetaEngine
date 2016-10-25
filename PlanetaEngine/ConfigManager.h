#pragma once

#include "IConfigManager.h"
#include "NonCopyable.h"

namespace planeta {
	class File;
	namespace private_ {
		class ConfigManager : public IConfigManager, private util::NonCopyable<ConfigManager> {
		public:
			virtual ~ConfigManager()override = default;
			virtual bool LoadSystemConfig(const File& file) = 0;
			virtual bool LoadUserConfig(const File& file) = 0;
			/*! カラービット深度を取得する*/
			virtual int color_bit_depth()const = 0;
			/*! スタートアップシーンIDを取得する*/
			virtual std::string startup_scene_id()const = 0;
		};
	}
}
