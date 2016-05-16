#include <cassert>
#include "GameObjectBase.h"
#include "GameObjectManagerPublicInterface.h"
#include "SceneData.h"
#include "GameObjectComponent.h"
#include "GameObjectComponentSetUpData.h"
#include "SystemLog.h"

namespace planeta_engine {

	GameObjectBase::GameObjectBase() {}

	GameObjectBase::~GameObjectBase() = default;

	std::shared_ptr<IGameObject> GameObjectBase::GetSharedPointer() {
		assert(shared_from_this() != nullptr);
		return shared_from_this();
	}

	bool GameObjectBase::Activate() {
		return manager_connection_->RequestActivation();
	}

	bool GameObjectBase::Inactivate() {
		return manager_connection_->RequestInactivation();
	}

	void GameObjectBase::Dispose() {
		manager_connection_->RequestDisposal();
	}

	bool GameObjectBase::OnInitialized() {
		return true;
	}

	bool GameObjectBase::OnActivated() {
		return true;
	}

	bool GameObjectBase::OnInactivated() {
		return true;
	}

	bool GameObjectBase::OnDisposed() {
		return true;
	}

	bool GameObjectBase::ProcessActivation() {
		if (!OnActivated()) {
			PE_LOG_ERROR("GameObject‚Ì—LŒø‰»ˆ—‚É¸”s‚µ‚Ü‚µ‚½B");
			return false;
		}
		if (!component_holder_.DoAllWithCheck(&GameObjectComponent::Activate, true)) {
			PE_LOG_ERROR("GameObjectComponent‚Ì—LŒø‰»‚É¸”s‚µ‚Ü‚µ‚½B");
			return false;
		}
		activated_event_delegate_();
		return true;
	}

	bool GameObjectBase::ProcessInactivation() {
		inactivated_event_delegate_();
		if (!OnInactivated()) {
			PE_LOG_ERROR("GameObject‚Ì–³Œø‰»ˆ—‚É¸”s‚µ‚Ü‚µ‚½B");
			return false;
		}
		if (!component_holder_.DoAllWithCheck(&GameObjectComponent::InActivate, true)) {
			PE_LOG_ERROR("GameObjectComponent‚Ì–³Œø‰»‚É¸”s‚µ‚Ü‚µ‚½B");
			return false;
		}
		return true;
	}

	bool GameObjectBase::ProcessDisposal() {
		disposed_event_delegate_();
		if (!OnDisposed()) {
			PE_LOG_ERROR("GameObject‚Ì”jŠüˆ—‚É¸”s‚µ‚Ü‚µ‚½B");
			return false;
		}
		component_holder_.DoAll(&GameObjectComponent::Finalize);
		return true;
	}

	void GameObjectBase::SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn) {
		manager_connection_ = std::move(mgr_cnctn);
	}

	void GameObjectBase::SetSceneData(const utility::WeakPointer<core::SceneData>& scene_accessor) {
		scene_data_ = scene_accessor;
	}

	utility::WeakPointer<IGameObject> GameObjectBase::CreateGameObject(const std::string& id) {
		return scene_data_->game_object_manager_public_interface.CreateGameObject(id);
	}

	utility::WeakPointer<IGameObject> GameObjectBase::CreateAndActivateGameObject(const std::string& id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateGameObject(id);
		go->Activate();
		return go;
	}

	utility::DelegateConnection GameObjectBase::AddActivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(activated_event_delegate_);
	}

	utility::DelegateConnection GameObjectBase::AddInactivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(inactivated_event_delegate_);
	}

	utility::DelegateConnection GameObjectBase::AddDisposedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(disposed_event_delegate_);
	}

	void GameObjectBase::SetUpGameComponent(GameObjectComponent& com) {
		core::GameObjectComponentSetUpData rd{ this, scene_data_ };
		com.SystemSetUp(rd);
	}

	std::shared_ptr<GameObjectComponent> GameObjectBase::GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		return component_holder_.GetComponentByTypeInfo(ti, type_checker);
	}

	std::vector<std::shared_ptr<GameObjectComponent>> GameObjectBase::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		return std::move(component_holder_.GetAllComponentsByTypeInfo(ti, type_checker));
	}

}