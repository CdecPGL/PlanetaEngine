#pragma once

#include <functional>
#include <unordered_map>
#include "TransformSystem.hpp"

namespace plnt {
	namespace private_ {
		class StandardTransformSystem : public TransformSystem {
		public:
			StandardTransformSystem();
			~StandardTransformSystem();
			bool Initialize() override;
			void Update() override;
			void Finalize() override;

			void ApplyVelocity() override;

			int RegisterTransform2D(CTransform2D *transform2d) override;
			bool RemoveTransform2D(int id) override;
			int RegisterTransformGUI(CTransformGUI *transformgui);
			bool RemoveTransformGUI(int id);

		private:
			std::unordered_map<int, CTransform2D *> transform2d_map_;
			std::unordered_map<int, CTransformGUI *> transformgui_map_;
			int id_counter_2d_ = 0;
			int id_counter_gui_ = 0;
		};
	}
}
