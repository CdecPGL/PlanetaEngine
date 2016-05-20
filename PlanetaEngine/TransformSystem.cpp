#include "TransformSystem.h"
#include "CTransform2D.h"
#include "SystemLog.h"

namespace planeta_engine {
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
			//ApplyVelocity�ɓo�^�폜�֘A�̃R�[�h�͂Ȃ��͂��Ȃ̂ŁA���̃��[�v���ł�T2D�̓o�^�폜(t2d_list�̕ύX)�͔������Ȃ��B
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
				PE_LOG_FATAL("�o�^����Ă��Ȃ�CTransform2D���w�肳��܂����BID:", id);
				return false;
			}
		}

	}
}