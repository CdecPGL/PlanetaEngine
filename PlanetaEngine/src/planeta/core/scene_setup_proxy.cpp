#include "scene_setup_proxy.hpp"
#include "scene.hpp"

namespace plnt {
	scene_setup_proxy::scene_setup_proxy(private_::scene &scene) : scene_(scene) { }

	i_scene &scene_setup_proxy::scene() const { return scene_; }
}
