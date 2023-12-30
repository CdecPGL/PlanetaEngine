#pragma once

#include "..\core\game_object_system_component.hpp"
#include "planeta/core/WeakPointer.hpp"
#include "..\core\graph_draw_data_2d.hpp"
#include "planeta/core/Vector2D.hpp"

namespace plnt {
	class screen_drawer_2d;
	class CTransform2D;

	/*! 描画基底コンポーネント*/
	class CDraw2D : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDraw2D);

	public:
		using super = private_::game_object_system_component;
		CDraw2D();
		virtual ~CDraw2D();
		/*描画処理*/
		void Draw(screen_drawer_2d &drawer);
		/*Accessor*/
		/*! @brief 描画優先度を取得
		
			値が大きいほど手前に表示される*/
		int draw_priority() const { return draw_priority_; }
		/*! @brief 描画優先度を設定
		
			値が大きいほど手前に表示される*/
		CDraw2D &draw_priority(int priority);
		/*! 表示位置を取得(CTransform2Dからの相対値)*/
		const vector_2dd &relative_position() const { return position_; }
		/*表示位置を設定(CTransform2Dからの相対値)*/
		CDraw2D &relative_position(const vector_2dd &pos) {
			position_ = pos;
			return *this;
		}

		/*表示回転度を取得(CTransform2Dからの相対値)*/
		double relative_rotation_rad() const { return rotation_rad_; }
		/*表示回転度を設定(CTransform2Dからの相対値)*/
		CDraw2D &relative_rotation_rad(double rota_rad) {
			rotation_rad_ = rota_rad;
			return *this;
		}

		/*! 表示拡大度を取得(CTransform2Dからの相対値)*/
		const vector_2dd &relative_scale() const { return scale_; }
		/*! 表示拡大度を設定(CTransform2Dからの相対値)*/
		CDraw2D &relative_scale(const vector_2dd &s) {
			scale_ = s;
			return *this;
		}

		/*! 表示色を取得*/
		plnt::color color() const { return color_; }
		/*! 表示色を設定*/
		CDraw2D &color(const plnt::color &c) {
			color_ = c;
			return *this;
		}

	protected:
		/*描画の中心位置取得(ゲームオブジェクトの形状情報と、表示位置から算出)*/
		vector_2dd GetDrawCenterPosition() const;
		/*描画回転度取得(ゲームオブジェクトの回転度と、表示回転度から算出)*/
		double GetDrawRotationRed() const;
		/*描画拡大度取得(ゲームオブジェクトの拡大度と、表示回拡大度から算出)*/
		vector_2dd GetDrawScale() const;

		non_owing_pointer<CTransform2D> transform2d_;

		virtual bool get_other_components_proc(const go_component_getter &com_getter) override;

		void on_initialized() override;
		void on_finalized() noexcept override;

	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		int draw_priority_ = 0; //描画優先度
		/*表示位置*/
		vector_2dd position_;
		/*表示回転度*/
		double rotation_rad_ = 0.0;
		/*表示拡大度*/
		vector_2dd scale_ = vector_2dd(1.0, 1.0);
		plnt::color color_;
		void UpdatePriority_();
		void on_activated() override final;
		void on_inactivated() override final;
		/*描画処理*/
		virtual void DrawProc(screen_drawer_2d &drawer) = 0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CDraw2D);
}
