#include "GameObjectComponent.h"
#include "GameObjectBase.h"
#include "GameObjectComponentSetUpData.h"

namespace planeta{
		bool GameObjectComponent::Initialize() {
			if (!OnInitialized()) {
				PE_LOG_ERROR("コンポーネントの初期化処理に失敗しました。");
				return false;
			}
			is_valied_ = true;
			return true;
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

		bool GameObjectComponent::SetSceneAndHolderGOData(const private_::GameObjectComponentSetUpData& resistration_data) {
			game_object_.reset(resistration_data.holder_game_object);
			SetSceneData(resistration_data.scene_data);
			return true;
		}

		IGameObject& GameObjectComponent::game_object() {
			return *game_object_;
		}
}
