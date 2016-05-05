#include "GameObjectComponent.h"
#include "GameObjectBase.h"
#include "GameObjectComponentSetUpData.h"

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

		bool GameObjectComponent::SystemSetUp(const core::GameObjectComponentSetUpData& resistration_data) {
			game_object_.reset(resistration_data.holder_game_object);
			SetSceneData(resistration_data.scene_data);
			return true;
		}

		IGameObjectForComponent& GameObjectComponent::game_object() {
			return *game_object_;
		}

}
