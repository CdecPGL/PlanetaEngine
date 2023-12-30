#include "SceneSetUpProxy.hpp"
#include "Scene.hpp"

namespace plnt {
	scene_set_up_proxy::scene_set_up_proxy(private_::scene &scene) : scene_(scene) { }

	i_scene &scene_set_up_proxy::scene() const { return scene_; }
}
