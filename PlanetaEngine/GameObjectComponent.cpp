#include "GameObjectComponent.h"
#include "GameObjectComponentSetUpData.h"
#include "LogUtility.h"

namespace planeta {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(GameObjectComponent) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(GameObjectComponent, labels)
			.ShallowCopyTarget(&GameObjectComponent::labels_);
	}

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

	const std::unordered_set<std::string>& GameObjectComponent::labels() const {
		return labels_;
	}

	void GameObjectComponent::labels(const std::unordered_set<std::string>& l) {
		labels_ = l;
	}

}
