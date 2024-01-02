#include "StandardTransformSystem.hpp"
#include "planeta/buildin/CTransform2D.hpp"
#include "log_utility.hpp"

namespace plnt {
	namespace private_ {
		StandardTransformSystem::StandardTransformSystem() { }


		StandardTransformSystem::~StandardTransformSystem() { }

		bool StandardTransformSystem::initialize() { return true; }

		void StandardTransformSystem::update() { }

		void StandardTransformSystem::finalize() { }

		void StandardTransformSystem::apply_velocity() {
			//ApplyVelocityに登録削除関連のコードはないはずなので、このループ内ではT2Dの登録削除(t2d_listの変更)は発生しない。
			for (auto &&t2d : transform2d_map_) { t2d.second->ApplyVelocity_(); }
		}

		int StandardTransformSystem::register_transform_2d(CTransform2D *transform2d) {
			int id = id_counter_2d_++;
			transform2d_map_.emplace(id, transform2d);
			return id;
		}

		bool StandardTransformSystem::remove_transform_2d(int id) {
			auto it = transform2d_map_.find(id);
			if (it != transform2d_map_.end()) {
				transform2d_map_.erase(it);
				return true;
			} else {
				PE_LOG_FATAL("登録されていないCTransform2Dが指定されました。ID:", id);
				return false;
			}
		}

		int StandardTransformSystem::register_transform_gui(CTransformGUI *transformgui) {
			int id = id_counter_gui_++;
			transformgui_map_.emplace(id, transformgui);
			return id;
		}

		bool StandardTransformSystem::remove_transform_gui(int id) {
			auto it = transformgui_map_.find(id);
			if (it != transformgui_map_.end()) {
				transformgui_map_.erase(it);
				return true;
			} else {
				PE_LOG_FATAL("登録されていないCTransformGUIが指定されました。ID:", id);
				return false;
			}
		}
	}
}
