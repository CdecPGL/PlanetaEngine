#pragma once

#include "../core/game_object_system_component.hpp"
#include "planeta/core/rectangle.hpp"

namespace plnt {
	//! GUI用トランスフォームコンポーネント(親子関係関連の機能は未実装)
	class c_transform_gui final : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_transform_gui);

	public:
		using super = game_object_system_component;
		c_transform_gui();
		~c_transform_gui() override;
		/*アクセサ*/
		//! 位置を取得。Pivotを基準点とする
		[[nodiscard]] const vector_2di &position() const;
		//! 位置を設定。Pivotを基準点とする
		c_transform_gui &position(const vector_2di &pos);
		//! 回転度を取得。Pivotを中心とする
		[[nodiscard]] double rotation_rad() const;
		//! 回転度を設定。Pivotを中心とする
		c_transform_gui &rotation_rad(double rota_rad);
		//! サイズを取得。Pivotを基準点とする
		[[nodiscard]] const vector_2di &size() const;
		//! サイズを設定。Pivotを基準点とする
		c_transform_gui &size(const vector_2di &s);
		//! 変形中心位置を取得(0.0-1.0)
		[[nodiscard]] const vector_2dd &pivot() const;
		//! 変形中心位置を設定(0.0-1.0)
		c_transform_gui &pivot(const vector_2dd &p);
		//! GUI領域を表す矩形を取得。回転は考慮されない
		//const rectangle_i& rect()const;
		//! アンカー左上の位置を取得。親CTransformGUI中での位置を(0.0-1.0)^2で表す
		//vector_2dd anchor_min()const;
		//! アンカー左上の位置を設定。親CTransformGUI中での位置を(0.0-1.0)^2で表す
		//CTransformGUI& anchor_min(const vector_2dd& a);
		//! アンカー右下の位置を取得。親CTransformGUI中での位置を(0.0-1.0)^2で表す
		//vector_2dd anchor_max()const;
		//! アンカー右下の位置を設定。親CTransformGUI中での位置を(0.0-1.0)^2で表す
		//CTransformGUI& anchor_max(const vector_2dd& a);
	private:
		class impl;
		std::unique_ptr<impl> impl_;
		//GOCオーバーライド
		void on_activated() override;
		void on_inactivated() override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_transform_gui);
}
