#pragma once

#include "..\core\game_object_system_component.hpp"
#include "planeta/core/rectangle.hpp"

namespace plnt {
	//! GUI用トランスフォームコンポーネント(親子関係関連の機能は未実装)
	class CTransformGUI final : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CTransformGUI);

	public:
		using super = private_::game_object_system_component;
		CTransformGUI();
		~CTransformGUI();
		/*アクセサ*/
		//! 位置を取得。Pivotを基準点とする
		const vector_2di &position() const;
		//! 位置を設定。Pivotを基準点とする
		CTransformGUI &position(const vector_2di &pos);
		//! 回転度を取得。Pivotを中心とする
		const double rotation_rad() const;
		//! 回転度を設定。Pivotを中心とする
		CTransformGUI &rotation_rad(double rota_rad);
		//! サイズを取得。Pivotを基準点とする
		const vector_2di &size() const;
		//! サイズを設定。Pivotを基準点とする
		CTransformGUI &size(const vector_2di &s);
		//! 変形中心位置を取得(0.0-1.0)
		const vector_2dd &pivot() const;
		//! 変形中心位置を設定(0.0-1.0)
		CTransformGUI &pivot(const vector_2dd &p);
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
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOCオーバーライド
		void on_activated() override final;
		void on_inactivated() override final;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CTransformGUI);
}
