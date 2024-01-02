﻿#pragma once

#include <list>

#include "..\core\game_object_standard_component.hpp"
#include "..\core\vector_2d.hpp"
#include "planeta/core/rectangle.hpp"
#include "planeta/core/i_collider_with_collider_2d.hpp"
#include "..\math\math_constant.hpp"

namespace plnt {
	class CCircleCollider2D;
	class CStraightLineCollider2D;
	class CTransform2D;

	/*! 地面基底コンポーネント*/
	class CGround2D :
		public game_object_standard_component, public private_::i_collider_with_collider_2d {
	public:
		using super = game_object_standard_component;
		CGround2D();
		virtual ~CGround2D() = default;
		/*! 指定地形座標の地面からの高さを取得*/
		virtual double GetAltitudeWithGroundPosition(const vector_2dd &ground_pos) const = 0;
		/*! グローバル座標を地形座標に変換*/
		virtual vector_2dd ConvertPositionGlobalToGround(const vector_2dd &global_pos) const = 0;
		/*! 地形座標をグローバル座標に変換*/
		virtual vector_2dd ConvertPositionGroundToGlobal(const vector_2dd &ground_pos) const = 0;
		/*! 指定地形座標でグローバル回転度を地形回転度に変換*/
		double ConvertRotationGlobalToGroundWithGroundPosition(const vector_2dd &ground_pos,
		                                                       double global_rota_rad) const;
		/*! 指定地形座標で地形回転度をグローバル回転度に変換*/
		double ConvertRotationGroundToGlobalWithGroundPosition(const vector_2dd &ground_pos,
		                                                       double ground_rota_rad) const;
		/*! 指定地形座標でグローバル速度を地形速度に変換*/
		vector_2dd ConvertVelocityGlobalToGroundWithGroundPosition(const vector_2dd &ground_pos,
		                                                          const vector_2dd &global_velocity);
		/*! 指定地形座標で地形速度をグローバル速度に変換*/
		vector_2dd ConvertVelocityGroundToGlobalWithGroundPosition(const vector_2dd &ground_pos,
		                                                          const vector_2dd &ground_velocity);
		/*! 指定地形座標で地形座標系ベクトルをグローバル座標系での長さと一致するよう変換*/
		virtual vector_2dd NormalizeGroundVectorWithGroundPosition(const vector_2dd &ground_pos,
		                                                          const vector_2dd &ground_vector) const = 0;
		/*! 同じゲームオブジェクトのCtransform2Dを取得する*/
		CTransform2D &transform2d();
		/*! 同じゲームオブジェクトのCtransform2Dを取得する(const)*/
		const CTransform2D &transform2d() const;

	protected:
		virtual bool get_other_components_proc(const go_component_getter &com_getter) override;
		virtual void on_activated() override;
		virtual void on_inactivated() override;
		virtual void on_finalized() noexcept override;

	private:
		non_owing_pointer<CTransform2D> transform2d_;
		/*指定地形座標で地形座標系回転度のグローバル座標系回転度との差を取得する*/
		virtual double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const vector_2dd &ground_pos) const = 0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CGround2D);
}
