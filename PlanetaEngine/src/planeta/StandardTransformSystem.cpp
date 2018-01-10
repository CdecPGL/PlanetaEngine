#include "StandardTransformSystem.hpp"
#include "CTransform2D.hpp"
#include "LogUtility.hpp"

namespace plnt {
	namespace private_ {
		StandardTransformSystem::StandardTransformSystem() {}


		StandardTransformSystem::~StandardTransformSystem() {}

		bool StandardTransformSystem::Initialize() {
			return true;
		}

		void StandardTransformSystem::Update() {

		}

		void StandardTransformSystem::Finalize() {

		}

		void StandardTransformSystem::ApplyVelocity() {
			//ApplyVelocityに登録削除関連のコードはないはずなので、このループ内ではT2Dの登録削除(t2d_listの変更)は発生しない。
			for (auto&& t2d : transform2d_map_) {
				t2d.second->ApplyVelocity_();
			}
		}

		int StandardTransformSystem::RegisterTransform2D(CTransform2D* transform2d) {
			int id = id_counter_2d_++;
			transform2d_map_.emplace(id, transform2d);
			return id;
		}

		bool StandardTransformSystem::RemoveTransform2D(int id) {
			auto it = transform2d_map_.find(id);
			if (it != transform2d_map_.end()) {
				transform2d_map_.erase(it);
				return true;
			} else {
				PE_LOG_FATAL("登録されていないCTransform2Dが指定されました。ID:", id);
				return false;
			}
		}

		int StandardTransformSystem::RegisterTransformGUI(CTransformGUI* transformgui) {
			int id = id_counter_gui_++;
			transformgui_map_.emplace(id, transformgui);
			return id;
		}

		bool StandardTransformSystem::RemoveTransformGUI(int id) {
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