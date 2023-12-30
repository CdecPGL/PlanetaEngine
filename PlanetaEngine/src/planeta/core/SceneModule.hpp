#pragma once

#include "Object.hpp"
#include "WeakPointer.hpp"
#include "non_copyable.hpp"

namespace plnt {
	class i_debug_information_adder;

	namespace private_ {
		class i_scene_internal;

		class SceneModule : public object, private util::non_copyable<SceneModule> {
		public:
			virtual ~SceneModule() = default;
			virtual bool Initialize() { return true; };
			virtual void Finalize() { return; };
			virtual void Update() = 0;

			virtual void DebugInformationAddHandle(i_debug_information_adder &di_adder) { };

			void SetScene(const WeakPointer<i_scene_internal> &scene) { scene_ = scene; }

		protected:
			WeakPointer<i_scene_internal> scene_internal_interface() { return scene_; }

		private:
			WeakPointer<i_scene_internal> scene_;
		};
	}
}
