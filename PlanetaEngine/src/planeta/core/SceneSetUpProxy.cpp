#include "planeta/core/SceneSetUpProxy.hpp"
#include "Scene.hpp"

namespace plnt {
	SceneSetUpProxy::SceneSetUpProxy(private_::Scene& pscene) :scene_(pscene) {

	}

	plnt::IScene& SceneSetUpProxy::scene() {
		return scene_;
	}

}