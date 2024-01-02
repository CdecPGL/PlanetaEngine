#pragma once

#include <functional>
#include <unordered_map>
#include "transform_system.hpp"

namespace plnt {
	namespace private_ {
		class StandardTransformSystem : public transform_system {
		public:
			StandardTransformSystem();
			~StandardTransformSystem();
			bool initialize() override;
			void update() override;
			void finalize() override;

			void apply_velocity() override;

			int register_transform_2d(CTransform2D *transform2d) override;
			bool remove_transform_2d(int id) override;
			int register_transform_gui(CTransformGUI *transformgui);
			bool remove_transform_gui(int id);

		private:
			std::unordered_map<int, CTransform2D *> transform2d_map_;
			std::unordered_map<int, CTransformGUI *> transformgui_map_;
			int id_counter_2d_ = 0;
			int id_counter_gui_ = 0;
		};
	}
}
