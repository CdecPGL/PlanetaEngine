#pragma once
#include "Object.h"

namespace planeta {
	namespace core {
		class Scene;
		/*SceneSetUpper���K�p�����O�ɃV�[���ɓK�p�����*/
		class SceneSystemSetUpper : public Object {
		public:
			bool operator()(Scene& scene);
		};
	}
}
