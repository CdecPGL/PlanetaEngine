#include <cassert>
#include "GameObjectBase.h"

namespace planeta_engine {

	std::shared_ptr<IGameObject> GameObjectBase::GetSharedPointer() const {
		assert(this_weak_ptr_.expired() == false);
		return this_weak_ptr_.lock();
	}

	bool GameObjectBase::Activate() {

	}

	bool GameObjectBase::Inactivate() {

	}

	void GameObjectBase::Dispose() {

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
}