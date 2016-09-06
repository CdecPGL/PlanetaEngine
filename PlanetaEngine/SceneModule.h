#pragma once

#include "Object.h"
#include "WeakPointer.h"

namespace planeta {
	namespace private_ {
		class ScenePublicInterface;
		struct SceneData;
		class SceneModule : public Object{
		public:
			virtual ~SceneModule() = default;
			virtual bool Initialize() { return true; };
			virtual void Finalize() { return; };
			virtual void Update() = 0;

			virtual void SetSceneData(const util::WeakPointer<private_::SceneData>& scene_data) {};
		private:

		};
	}
}
