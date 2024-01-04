#pragma once

#include "../core/game_object_system_component.hpp"
#include "../core/graph_draw_data_2d.hpp"
#include "../core/vector_2d.hpp"

namespace plnt {
	class screen_drawer_2d;
	class c_transform_2d;

	/*! 描画基底コンポーネント*/
	class c_draw_2d : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_draw_2d);

	public:
		using super = game_object_system_component;
		c_draw_2d();
		~c_draw_2d() override;
		/*描画処理*/
		void draw(screen_drawer_2d &drawer);
		/*Accessor*/
		/*! @brief 描画優先度を取得
		
			値が大きいほど手前に表示される*/
		[[nodiscard]] int draw_priority() const { return draw_priority_; }
		/*! @brief 描画優先度を設定
		
			値が大きいほど手前に表示される*/
		c_draw_2d &draw_priority(int priority);
		/*! 表示位置を取得(CTransform2Dからの相対値)*/
		[[nodiscard]] const vector_2dd &relative_position() const { return position_; }
		/*表示位置を設定(CTransform2Dからの相対値)*/
		c_draw_2d &relative_position(const vector_2dd &pos) {
			position_ = pos;
			return *this;
		}

		/*表示回転度を取得(CTransform2Dからの相対値)*/
		[[nodiscard]] double relative_rotation_rad() const { return rotation_rad_; }
		/*表示回転度を設定(CTransform2Dからの相対値)*/
		c_draw_2d &relative_rotation_rad(double rota_rad) {
			rotation_rad_ = rota_rad;
			return *this;
		}

		/*! 表示拡大度を取得(CTransform2Dからの相対値)*/
		[[nodiscard]] const vector_2dd &relative_scale() const { return scale_; }
		/*! 表示拡大度を設定(CTransform2Dからの相対値)*/
		c_draw_2d &relative_scale(const vector_2dd &s) {
			scale_ = s;
			return *this;
		}

		/*! 表示色を取得*/
		[[nodiscard]] color color() const { return color_; }
		/*! 表示色を設定*/
		c_draw_2d &color(const plnt::color &c) {
			color_ = c;
			return *this;
		}

	protected:
		/*描画の中心位置取得(ゲームオブジェクトの形状情報と、表示位置から算出)*/
		[[nodiscard]] vector_2dd get_draw_center_position() const;
		/*描画回転度取得(ゲームオブジェクトの回転度と、表示回転度から算出)*/
		[[nodiscard]] double get_draw_rotation_red() const;
		/*描画拡大度取得(ゲームオブジェクトの拡大度と、表示回拡大度から算出)*/
		[[nodiscard]] vector_2dd get_draw_scale() const;

		weak_pointer<c_transform_2d> transform2d_;

		bool get_other_components_proc(const go_component_getter &com_getter) override;

		void on_initialized() override;
		void on_finalized() noexcept override;

	private:
		class impl;
		std::unique_ptr<impl> impl_;

		int draw_priority_ = 0; //描画優先度
		/*表示位置*/
		vector_2dd position_;
		/*表示回転度*/
		double rotation_rad_ = 0.0;
		/*表示拡大度*/
		vector_2dd scale_ = vector_2dd(1.0, 1.0);
		plnt::color color_;
		void update_priority() const;
		void on_activated() final;
		void on_inactivated() final;
		/*描画処理*/
		virtual void draw_proc(screen_drawer_2d &drawer) = 0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_draw_2d);
}
