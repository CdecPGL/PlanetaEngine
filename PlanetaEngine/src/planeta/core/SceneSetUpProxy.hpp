#pragma once

namespace plnt {
	class i_scene;

	namespace private_ {
		class Scene;
	}

	class SceneSetUpProxy {
	public:
		SceneSetUpProxy(private_::Scene &pscene);
		i_scene &scene();

	private:
		private_::Scene &scene_;
	};
}
