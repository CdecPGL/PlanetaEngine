#pragma once

#include "IConfigManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	class File;

	namespace private_ {
		class config_manager : public IConfigManager, public SubSystemManager {
		public:
			config_manager() = default;
			config_manager(const config_manager &) = delete;
			config_manager(config_manager &&) = delete;
			~config_manager() override = default;
			config_manager &operator=(const config_manager &) = delete;
			config_manager &operator=(config_manager &&) = delete;

			virtual bool load_system_config(const File &file) = 0;
			virtual bool load_user_config(const File &file) = 0;
			/*! カラービット深度を取得する*/
			virtual int color_bit_depth() const = 0;
			/*! スタートアップシーンIDを取得する*/
			virtual std::string startup_scene_id() const = 0;
		};
	}
}
