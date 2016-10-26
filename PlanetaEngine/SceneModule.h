#pragma once

#include "Object.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta {
	namespace private_ {
		class ScenePublicInterface;
		struct SceneData;
		class SceneModule : public Object, private util::NonCopyable<SceneModule>{
		public:
			virtual ~SceneModule() = default;
			virtual bool Initialize() { return true; };
			virtual void Finalize() { return; };
			virtual void Update() = 0;

			virtual void SetSceneData(const WeakPointer<private_::SceneData>& scene_data) {};
		private:

		};
	}
}
