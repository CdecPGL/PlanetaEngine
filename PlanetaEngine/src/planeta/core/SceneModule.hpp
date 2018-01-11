#pragma once

#include "planeta/core/Object.hpp"
#include "planeta/core/WeakPointer.hpp"
#include "NonCopyable.hpp"

namespace plnt {
	class IDebugInformationAdder;
	namespace private_ {
		class ISceneInternal;
		class SceneModule : public Object, private util::NonCopyable<SceneModule>{
		public:
			virtual ~SceneModule() = default;
			virtual bool Initialize() { return true; };
			virtual void Finalize() { return; };
			virtual void Update() = 0;
			virtual void DebugInformationAddHandle(IDebugInformationAdder& di_adder) {};
			void SetScene(const WeakPointer<ISceneInternal>& scene) {
				scene_ = scene;
			}
		protected:
			WeakPointer<ISceneInternal> scene_internal_interface() { return scene_; }
		private:
			WeakPointer<ISceneInternal> scene_;
		};
	}
}
