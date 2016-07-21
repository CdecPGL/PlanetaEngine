#include "TransformSystem.h"
#include "CTransform2D.h"
#include "SystemLog.h"

namespace planeta {
	namespace core {
		TransformSystem::TransformSystem() {}


		TransformSystem::~TransformSystem() {}

		bool TransformSystem::Initialize() {
			return true;
		}

		void TransformSystem::Update() {

		}

		void TransformSystem::Finalize() {

		}

		void TransformSystem::ApplyVelocity() {
			//ApplyVelocityに登録削除関連のコードはないはずなので、このループ内ではT2Dの登録削除(t2d_listの変更)は発生しない。
			for (auto&& t2d : transform2d_map_) {
				t2d.second->ApplyVelocity_();
			}
		}

		int TransformSystem::RegisterTransform2D(CTransform2D* transform2d) {
			int id = id_counter_++;
			transform2d_map_.emplace(id, transform2d);
			return id;
		}

		bool TransformSystem::RemoveTransform2D(int id) {
			auto it = transform2d_map_.find(id);
			if (it != transform2d_map_.end()) {
				transform2d_map_.erase(it);
				return true;
			} else {
				PE_LOG_FATAL("登録されていないCTransform2Dが指定されました。ID:", id);
				return false;
			}
		}

	}
}