#pragma once
#include "Object.h"

namespace planeta_engine {
	namespace core {
		class Scene;
		/*SceneSetUpperが適用される前にシーンに適用される*/
		class SceneSystemSetUpper : public Object {
		public:
			bool operator()(Scene& scene);
		};
	}
}
