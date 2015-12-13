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

		bool GameObjectComponent::SystemSetUp(const core::GameObjectComponentRegistrationData& resistration_data, const core::SceneDataForGameObject& special_setup_data) {
			scene_accessor_ = resistration_data.scene_accessor;
			game_object_ = resistration_data.holder_game_object;
			id_ = resistration_data.id;
			return SpecialSetUp(special_setup_data);
		}
}
