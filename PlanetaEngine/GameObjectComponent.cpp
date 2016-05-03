#include "GameObjectComponent.h"
#include "GameObject.h"
#include "GameObjectComponentRigistrationData.h"

namespace planeta_engine{
		bool GameObjectComponent::Initialize() {
			if (OnInitialized()) {
				is_valied_ = true;
				return true;
			} else {
				return false;
			}
		}

		void GameObjectComponent::Finalize() {
			OnFinalized();
			is_valied_ = false;
		}

		bool GameObjectComponent::Activate() {
			if (is_active_) { return true; }
			if (OnActivated()) {
				is_active_ = true;
				return true;
			} else {
				return false;
			}
		}

		bool GameObjectComponent::InActivate() {
			if (!is_active_) { return true; }
			if (OnInactivated()) {
				is_active_ = false;
				return true;
			} else {
				return false;
			}
		}

		bool GameObjectComponent::SystemSetUp(const core::GameObjectComponentRegistrationData& resistration_data, const utility::WeakPointer<core::SceneData>& scene_data) {
			SetSceneData(scene_data);
			return true;
		}
}
