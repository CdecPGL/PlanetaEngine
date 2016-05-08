#pragma once

#include "Object.h"
#include "WeakPointer.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
		struct SceneData;
		class SceneModule : public Object{
		public:
			virtual ~SceneModule() = default;
			virtual bool Initialize() { return true; };
			virtual void Finalize() { return; };
			virtual void Update() = 0;

			virtual void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data) {};
		private:

		};
	}
}
