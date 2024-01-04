#include <ranges>

#include "standard_transform_system.hpp"
#include "..\buildin\c_transform_2d.hpp"
#include "log_utility.hpp"

namespace plnt::private_ {
	bool standard_transform_system::initialize() { return true; }

	void standard_transform_system::update() {}

	void standard_transform_system::finalize() {}

	void standard_transform_system::apply_velocity() {
		//ApplyVelocityに登録削除関連のコードはないはずなので、このループ内ではT2Dの登録削除(t2d_listの変更)は発生しない。
		for (const auto &t2d : transform2d_map_ | std::views::values) { t2d->apply_velocity(); }
	}

	int standard_transform_system::register_transform_2d(c_transform_2d *transform2d) {
		int id = id_counter_2d_++;
		transform2d_map_.emplace(id, transform2d);
		return id;
	}

	bool standard_transform_system::remove_transform_2d(int id) {
		if (const auto it = transform2d_map_.find(id); it != transform2d_map_.end()) {
			transform2d_map_.erase(it);
			return true;
		}
		PE_LOG_FATAL("登録されていないCTransform2Dが指定されました。ID:", id);
		return false;
	}

	int standard_transform_system::register_transform_gui(c_transform_gui *transform_gui) {
		int id = id_counter_gui_++;
		transform_gui_map_.emplace(id, transform_gui);
		return id;
	}

	bool standard_transform_system::remove_transform_gui(int id) {
		if (const auto it = transform_gui_map_.find(id); it != transform_gui_map_.end()) {
			transform_gui_map_.erase(it);
			return true;
		}
		PE_LOG_FATAL("登録されていないCTransformGUIが指定されました。ID:", id);
		return false;
	}
}
