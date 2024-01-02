#pragma once

#include "i_log_manager.hpp"
#include "SubSystemManager.hpp"

namespace plnt::private_ {
	/*ログマネージャ*/
	class log_manager : public i_log_manager, public sub_system_manager {
	public:
		log_manager() = default;
		log_manager(const log_manager &) = delete;
		log_manager(log_manager &&) = delete;
		~log_manager() override = default;
		log_manager &operator=(const log_manager &) = delete;
		log_manager &operator=(log_manager &&) = delete;

		/*初期化*/
		virtual bool initialize() = 0;
		/*終了処理*/
		virtual void finalize() = 0;
	};
}
