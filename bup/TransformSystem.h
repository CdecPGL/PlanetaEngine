#pragma once

#include <functional>
#include <unordered_map>
#include "SceneModule.h"

namespace planeta {
	class CTransform2D;
	namespace core {
		class TransformSystem  : public SceneModule{
		public:
			TransformSystem();
			~TransformSystem();
			bool Initialize()override;
			void Update()override;
			void Finalize()override;

			void ApplyVelocity();

			int RegisterTransform2D(CTransform2D* transform2d);
			bool RemoveTransform2D(int id);
		private:
			std::unordered_map<int, CTransform2D*> transform2d_map_;
			int id_counter_ = 0;
		};
	}
}
