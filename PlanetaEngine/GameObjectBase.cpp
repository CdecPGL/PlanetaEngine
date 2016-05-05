#include <cassert>
#include "GameObjectBase.h"
#include "GameObjectManagerConnection.h"
#include "GameObjectManagerPublicInterface.h"
#include "SceneData.h"

namespace planeta_engine {

	GameObjectBase::GameObjectBase() :component_holder_(*this) {}

	GameObjectBase::~GameObjectBase() = default;

	std::shared_ptr<IGameObject> GameObjectBase::GetSharedPointer() const {
		assert(this_weak_ptr_.expired() == false);
		return this_weak_ptr_.lock();
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
		return false;
	}

	bool GameObjectBase::ProcessInactivation() {
		return false;
	}

	bool GameObjectBase::ProcessDisposal() {
		return false;
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

	GameObjectComponentHolder& GameObjectBase::RefComponentHolder() {
		return component_holder_;
	}

}