#pragma once

namespace plnt {
	class i_scene;

	namespace private_ {
		class scene;
	}

	class scene_set_up_proxy {
	public:
		explicit scene_set_up_proxy(private_::scene &scene);
		[[nodiscard]] i_scene &scene() const;

	private:
		private_::scene &scene_;
	};
}
