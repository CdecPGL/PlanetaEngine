#pragma once

#include "../core/game_object_standard_component.hpp"

namespace plnt {
	class c_transform_2d;

	/*! 地面方向への重力発生コンポーネント*/
	class c_belonging_ground_gravity final : public game_object_standard_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_belonging_ground_gravity);

	public:
		using super = game_object_standard_component;
		/*! 重力スケールを取得*/
		[[nodiscard]] double gravity_scale() const { return gravity_scale_; }
		/*! 重力スケールを設定*/
		[[nodiscard]] c_belonging_ground_gravity &gravity_scale(const double gs) {
			gravity_scale_ = gs;
			return *this;
		}

	private:
		non_owing_pointer<c_transform_2d> transform2d_ = {};
		double gravity_scale_ = 0.5;
		bool get_other_components_proc(const go_component_getter &com_getter) override;
		void on_initialized() override;
		void update() const;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_belonging_ground_gravity);
}
