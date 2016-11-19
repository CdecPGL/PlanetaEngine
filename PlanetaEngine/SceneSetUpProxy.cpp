#include "SceneSetUpProxy.h"
#include "Scene.h"

namespace planeta {
	SceneSetUpProxy::SceneSetUpProxy(private_::Scene& pscene) :scene_(pscene) {

	}

	planeta::IScene& SceneSetUpProxy::scene() {
		return scene_;
	}

}