#pragma once

#include <memory>

#include "i_save_manager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	class file_manipulator;

	namespace private_ {
		class save_manager : public i_save_manager, public SubSystemManager {
		public:
			save_manager() = default;
			save_manager(const save_manager &) = delete;
			save_manager(save_manager &&) = delete;
			~save_manager() override = default;
			save_manager &operator=(const save_manager &) = delete;
			save_manager &operator=(save_manager &&) = delete;

			//ファイルアクセサを設定。初期化前に呼び出す。
			virtual void set_file_manipulator(const std::shared_ptr<file_manipulator> &file_accessor) = 0;
			virtual bool initialize() = 0;
			virtual void finalize() = 0;
		};
	}
}
