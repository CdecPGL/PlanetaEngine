#include "GameObjectComponent.h"
#include "GameObject.h"

namespace planeta_engine{
	namespace game{

		

		bool GameObjectComponent::Initialize() {
			if (Initialize_()) {
				is_valied_ = true;
				return true;
			} else {
				return false;
			}
		}

		void GameObjectComponent::Finalize() {
			Finalize_();
			is_valied_ = false;
		}

		bool GameObjectComponent::Activate() {
			if (is_active_) { return true; }
			if (Activated_()) {
				is_active_ = true;
				return true;
			} else {
				return false;
			}
		}

		bool GameObjectComponent::InActivate() {
			if (!is_active_) { return true; }
			if (InActivated_()) {
				is_active_ = false;
				return true;
			} else {
				return false;
			}
		}

	}
}
