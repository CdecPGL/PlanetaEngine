#include <cassert>
#include "GameObjectBase.h"
#include "GameObjectManagerConnection.h"

namespace planeta_engine {

	GameObjectBase::GameObjectBase() = default;

	GameObjectBase::~GameObjectBase() = default;

	std::shared_ptr<IGameObject> GameObjectBase::GetSharedPointer() const {
		assert(this_weak_ptr_.expired() == false);
		return this_weak_ptr_.lock();
	}

	bool GameObjectBase::Activate() {
		manager_connection_->RequestActivation();
	}

	bool GameObjectBase::Inactivate() {
		manager_connection_->RequestInactivation();
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
		
	}

	bool GameObjectBase::ProcessInactivation() {
		
	}

	bool GameObjectBase::ProcessDisposal() {

	}

	void GameObjectBase::SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn) {
		manager_connection_ = std::move(mgr_cnctn);
	}

	void GameObjectBase::SetSceneAccessor(const utility::WeakPointer<SceneAccessorForGameObject>& scene_accessor) {
		scene_accessor_ = scene_accessor;
	}

}