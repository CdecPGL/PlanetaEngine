﻿#pragma once

#include <set>
#include <functional>

#include "planeta/core/signal.hpp"
#include "..\core\game_object_system_component.hpp"
#include "planeta/core/WeakPointer.hpp"
#include "..\core\vector_2d.hpp"
#include "planeta/core/i_collider_with_collider_2d.hpp"

namespace plnt {
	class CTransform2D;
	class e_collision_with_collider_2d;
	class e_collision_with_ground_2d;

	/*! コライダーの基底コンポーネント*/
	class CCollider2D : public private_::game_object_system_component, public private_::i_collider_with_collider_2d {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CCollider2D);

	public:
		using super = private_::game_object_system_component;
		virtual ~CCollider2D() = default;
		/*衝突判定を行う*/
		virtual bool DetectCollision(private_::i_collider_with_collider_2d &collider) = 0;

		/*グローバル中心位置取得*/
		const vector_2dd GetCollisionGlobalCenterPosition() const;
		/*拡大度取得(トランスフォームの拡大度xyで小さい方の要素が適用される)*/
		const double GetCollisionScale() const;
		/*グローバル回転度取得*/
		const double GetCollisionGlobalRotationRad() const;

		/*アクセサ*/
		/*! 相対位置を取得*/
		const vector_2dd &relative_position() const { return position_; }
		/*! 相対位置を設定*/
		CCollider2D &relative_position(const vector_2dd &pos) {
			position_ = pos;
			return *this;
		}

		/*! 相対回転度を取得*/
		const double relative_rotation_rad() const { return rotation_rad_; }
		/*! 相対回転度を設定*/
		CCollider2D &relative_rotation_rad(double rota_rad) {
			rotation_rad_ = rota_rad;
			return *this;
		}

		/*! 地形衝突フラグを取得*/
		const bool is_collidable_with_ground() const { return collide_with_ground_flag_; }
		/*! 地形衝突フラグを設定*/
		CCollider2D &is_collidable_with_ground(bool flag);
		/*! 衝突グループを取得*/
		const std::string &collision_group() const { return collision_group_name_; }
		/*! 衝突グループを設定*/
		CCollider2D &collision_group(const std::string &cg);
		/*! 接地しているかを取得*/
		bool is_grounded() const { return is_grounded_; }

		/*イベント*/
		/*! コライダーとの衝突イベント*/
		signal<void(const e_collision_with_collider_2d &)> collided_with_collider2d;
		/*! 2D地形との衝突イベントハンドラ追加*/
		signal<void(const e_collision_with_ground_2d &)> collided_with_ground2d;

		/*! 同じオブジェクトのCTransform2Dを取得*/
		CTransform2D &transform2d() { return *transform2d_; }
		/*! 同じオブジェクトのCTransform2Dを取得(const版)*/
		const CTransform2D &transform2d() const { return *transform2d_; }

	protected:
		virtual bool get_other_components_proc(const go_component_getter &com_getter) override;

	private:
		void on_activated() override final;
		void on_inactivated() override final;
		void ResistToCollisionDetectProcess_();
		void RemoveFromCollisionDetectProcess_();

		non_owing_pointer<CTransform2D> transform2d_;

		/*位置*/
		vector_2dd position_;
		/*回転度*/
		double rotation_rad_ = 0.0;
		/*地形衝突フラグ*/
		bool collide_with_ground_flag_ = false;
		/*衝突グループ名*/
		std::string collision_group_name_;
		/*接地しているか*/
		bool is_grounded_ = false;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CCollider2D);
}
