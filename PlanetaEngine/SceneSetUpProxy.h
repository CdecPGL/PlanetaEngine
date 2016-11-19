#pragma once

namespace planeta {
	class IScene;
	namespace private_ {
		class Scene;
	}
	class SceneSetUpProxy {
	public:
		SceneSetUpProxy(private_::Scene& pscene);
		IScene& scene();
	private:
		private_::Scene& scene_;
	};
}