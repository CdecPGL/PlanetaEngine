#pragma once

#include "i_config_manager.hpp"
#include "sub_system_manager.hpp"

namespace plnt {
	class file;

	namespace private_ {
		class config_manager : public i_config_manager, public sub_system_manager {
		public:
			config_manager() = default;
			config_manager(const config_manager &) = delete;
			config_manager(config_manager &&) = delete;
			~config_manager() override = default;
			config_manager &operator=(const config_manager &) = delete;
			config_manager &operator=(config_manager &&) = delete;

			virtual bool load_system_config(const file &file) = 0;
			virtual bool load_user_config(const file &file) = 0;
			/*! カラービット深度を取得する*/
			virtual int color_bit_depth() const = 0;
			/*! スタートアップシーンIDを取得する*/
			virtual std::string startup_scene_id() const = 0;
		};
	}
}
