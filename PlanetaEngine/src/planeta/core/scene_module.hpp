#pragma once

#include "object.hpp"
#include "weak_pointer.hpp"
#include "non_copyable.hpp"

namespace plnt {
	class i_debug_information_adder;

	namespace private_ {
		class i_scene_internal;

		// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
		class scene_module : public object, util::non_copyable<scene_module> {
		public:
			scene_module() = default;
			scene_module(const scene_module &) = delete;
			scene_module(scene_module &&) = delete;
			~scene_module() override = default;
			scene_module &operator=(scene_module &&) = delete;

			virtual bool initialize() { return true; }
			virtual void finalize() { }
			virtual void update() = 0;

			virtual void debug_information_add_handle(i_debug_information_adder &di_adder) { }

			void set_scene(const weak_pointer<i_scene_internal> &scene) { scene_ = scene; }

		protected:
			[[nodiscard]] weak_pointer<i_scene_internal> scene_internal_interface() { return scene_; }

		private:
			weak_pointer<i_scene_internal> scene_;
		};
	}
}
