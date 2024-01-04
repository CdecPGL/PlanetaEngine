#pragma once

#include "../core/game_object_standard_component.hpp"
#include "../core/vector_2d.hpp"
#include "planeta/core/i_collider_with_collider_2d.hpp"

namespace plnt {
	class c_circle_collider_2d;
	class c_straight_line_collider_2d;
	class c_transform_2d;

	/*! 地面基底コンポーネント*/
	class c_ground_2d :
		public game_object_standard_component, public private_::i_collider_with_collider_2d {
	public:
		using super = game_object_standard_component;
		c_ground_2d();
		~c_ground_2d() override = default;
		/*! 指定地形座標の地面からの高さを取得*/
		[[nodiscard]] virtual double get_altitude_with_ground_position(const vector_2dd &ground_pos) const = 0;
		/*! グローバル座標を地形座標に変換*/
		[[nodiscard]] virtual vector_2dd convert_position_global_to_ground(const vector_2dd &global_pos) const = 0;
		/*! 地形座標をグローバル座標に変換*/
		[[nodiscard]] virtual vector_2dd convert_position_ground_to_global(const vector_2dd &ground_pos) const = 0;
		/*! 指定地形座標でグローバル回転度を地形回転度に変換*/
		[[nodiscard]] double convert_rotation_global_to_ground_with_ground_position(const vector_2dd &ground_pos,
		                                                       double global_rota_rad) const;
		/*! 指定地形座標で地形回転度をグローバル回転度に変換*/
		[[nodiscard]] double convert_rotation_ground_to_global_with_ground_position(const vector_2dd &ground_pos,
		                                                       double ground_rota_rad) const;
		/*! 指定地形座標でグローバル速度を地形速度に変換*/
		[[nodiscard]] vector_2dd convert_velocity_global_to_ground_with_ground_position(const vector_2dd &ground_pos,
		                                                          const vector_2dd &global_velocity) const;
		/*! 指定地形座標で地形速度をグローバル速度に変換*/
		[[nodiscard]] vector_2dd convert_velocity_ground_to_global_with_ground_position(const vector_2dd &ground_pos,
		                                                          const vector_2dd &ground_velocity) const;
		/*! 指定地形座標で地形座標系ベクトルをグローバル座標系での長さと一致するよう変換*/
		[[nodiscard]] virtual vector_2dd normalize_ground_vector_with_ground_position(const vector_2dd &ground_pos,
		                                                          const vector_2dd &ground_vector) const = 0;
		/*! 同じゲームオブジェクトのc_transform_2dを取得する*/
		[[nodiscard]] c_transform_2d &transform2d();
		/*! 同じゲームオブジェクトのc_transform_2dを取得する(const)*/
		[[nodiscard]] const c_transform_2d &transform2d() const;

	protected:
		[[nodiscard]] bool get_other_components_proc(const go_component_getter &com_getter) override;
		void on_activated() override;
		void on_inactivated() override;
		void on_finalized() noexcept override;

	private:
		non_owing_pointer<c_transform_2d> transform2d_;
		/*指定地形座標で地形座標系回転度のグローバル座標系回転度との差を取得する*/
		[[nodiscard]] virtual double get_angle_difference_in_rad_ground_from_global_with_ground_position(const vector_2dd &ground_pos) const = 0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_ground_2d);
}
