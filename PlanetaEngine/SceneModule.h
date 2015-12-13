#pragma once

#include "Object.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
		struct SceneData;
		class SceneModule : public Object{
		public:
			virtual ~SceneModule() = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual void SetSceneInterface(ScenePublicInterface& scene_public_interface) = 0;
			virtual void SetSceneData(const SceneData& scene_data) = 0;
		private:

		};
	}
}
