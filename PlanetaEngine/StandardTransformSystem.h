#pragma once

#include <functional>
#include <unordered_map>
#include "TransformSystem.h"

namespace planeta {
	namespace private_ {
		class StandardTransformSystem  : public TransformSystem {
		public:
			StandardTransformSystem();
			~StandardTransformSystem();
			bool Initialize()override;
			void Update()override;
			void Finalize()override;

			void ApplyVelocity()override;

			int RegisterTransform2D(CTransform2D* transform2d)override;
			bool RemoveTransform2D(int id)override;
		private:
			std::unordered_map<int, CTransform2D*> transform2d_map_;
			int id_counter_ = 0;
		};
	}
}
