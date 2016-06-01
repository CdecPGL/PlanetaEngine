#include <cassert>
#include "GameObjectBase.h"
#include "GameObjectManagerPublicInterface.h"
#include "SceneData.h"
#include "GameObjectComponent.h"
#include "GameObjectComponentSetUpData.h"
#include "SystemLog.h"
#include "TGameObjectOperation.h"

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

	bool GameObjectBase::ProcessInitialization() {
		if (!OnInitialized()) {
			PE_LOG_ERROR("GameObject‚Ì‰Šú‰»ˆ—‚É¸”s‚µ‚Ü‚µ‚½B");
			return false;
		}
		if (!component_holder_.DoAllWithCheck(&GameObjectComponent::Initialize, true)) {
			PE_LOG_ERROR("GameObjectComponent‚Ì‰Šú‰»ˆ—‚É¸”s‚µ‚Ü‚µ‚½B");
			return false;
		}
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

	void GameObjectBase::SetSceneData(const util::WeakPointer<core::SceneData>& scene_accessor) {
		scene_data_ = scene_accessor;
	}

	util::WeakPointer<IGameObject> GameObjectBase::CreateGameObject(const std::string& id) {
		return scene_data_->game_object_manager_public_interface.CreateGameObject(id);
	}

	util::WeakPointer<IGameObject> GameObjectBase::CreateAndActivateGameObject(const std::string& id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateGameObject(id);
		if (go == nullptr) { return nullptr; }
		go->Activate();
		return go;
	}

	util::DelegateConnection GameObjectBase::AddActivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(activated_event_delegate_);
	}

	util::DelegateConnection GameObjectBase::AddInactivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(inactivated_event_delegate_);
	}

	util::DelegateConnection GameObjectBase::AddDisposedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) {
		return hander_adder(disposed_event_delegate_);
	}

	void GameObjectBase::SetUpGameComponent(GameObjectComponent& com) {
		core::GameObjectComponentSetUpData rd{ this, scene_data_ };
		com.SystemSetUp(rd);
	}

	std::shared_ptr<GameObjectComponent> GameObjectBase::GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		return component_holder_.GetComponentByTypeInfo(ti, type_checker);
	}

	std::vector<std::shared_ptr<GameObjectComponent>> GameObjectBase::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		return std::move(component_holder_.GetAllComponentsByTypeInfo(ti, type_checker));
	}

	TaskManagerPublicInterface& GameObjectBase::RefTaskManagerInterface_() {
		return scene_data_->task_manager_public_interface;
	}

	void GameObjectBase::SetUpAttachedTask_(TGameObjectOperation& task) {
		task.Attach(GetSharedPointer(), true);
	}
}