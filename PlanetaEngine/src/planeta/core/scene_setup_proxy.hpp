#pragma once

namespace plnt {
	class i_scene;

	namespace private_ {
		class scene;
	}

	class scene_setup_proxy {
	public:
		explicit scene_setup_proxy(private_::scene &scene);
		[[nodiscard]] i_scene &scene() const;

	private:
		private_::scene &scene_;
	};
}
