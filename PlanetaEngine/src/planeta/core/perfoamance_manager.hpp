#pragma once

#include "i_performance_manager.hpp"
#include "SubSystemManager.hpp"

namespace plnt::private_ {
	/*パフォーマンスマネージャ*/
	class performance_manager : public i_performance_manager, public SubSystemManager {
	public:
		performance_manager() = default;
		performance_manager(const performance_manager &) = delete;
		performance_manager(performance_manager &&) = delete;
		~performance_manager() override = default;
		performance_manager &operator=(const performance_manager &) = delete;
		performance_manager &operator=(performance_manager &&) = delete;

		virtual bool initialize() = 0;
		virtual void finalize() = 0;
		virtual void update() = 0;
	};
}
