#include "SceneSetUpProxy.hpp"
#include "Scene.hpp"

namespace plnt {
	SceneSetUpProxy::SceneSetUpProxy(private_::Scene &pscene) : scene_(pscene) { }

	plnt::i_scene &SceneSetUpProxy::scene() { return scene_; }
}
