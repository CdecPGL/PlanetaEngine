#include "GameObjectComponent.hpp"
#include "GameObjectComponentSetUpData.hpp"
#include "LogUtility.hpp"
#include "ISceneInternal.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(GameObjectComponent) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(GameObjectComponent, labels)
			.ShallowCopyTarget(&GameObjectComponent::labels_);
	}

	void GameObjectComponent::Initialize() {
		OnInitialized();
		is_valied_ = true;
	}

	void GameObjectComponent::Finalize() {
		OnFinalized();
		is_valied_ = false;
	}

	void GameObjectComponent::Activate() {
		if (is_active_) {
			PE_LOG_WARNING("すでに有効なゲームオブジェクトコンポーネントを有効化しようとしました。type:", typeid(*this).name());
			return;
		}
		OnActivated();
		is_active_ = true;
	}

	void GameObjectComponent::InActivate() {
		if (!is_active_) {
			PE_LOG_WARNING("すでに無効なゲームオブジェクトコンポーネントを無効化しようとしました。type:", typeid(*this).name());
			return;
		}
		OnInactivated();
		is_active_ = false;
	}

	bool GameObjectComponent::SetSceneAndHolderGOData(const private_::GameObjectComponentSetUpData &resistration_data) {
		game_object_.reset(resistration_data.holder_game_object);
		scene_ = resistration_data.scene_internal_interface;
		SetSceneInternalInterface(resistration_data.scene_internal_interface);
		return true;
	}

	IGameObject &GameObjectComponent::game_object() { return *game_object_; }

	const std::unordered_set<std::string> &GameObjectComponent::labels() const { return labels_; }

	void GameObjectComponent::labels(const std::unordered_set<std::string> &l) { labels_ = l; }

	IScene &GameObjectComponent::scene() { return *scene_; }
}
