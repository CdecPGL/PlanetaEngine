#pragma once

#include "planeta/GameObjectSystemComponent.hpp"
#include "planeta/RectAngle.hpp"

namespace plnt {
	//! GUI用トランスフォームコンポーネント(親子関係関連の機能は未実装)
	class CTransformGUI final : public private_::GameObjectSystemComponent{
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CTransformGUI);
	public:
		using Super = private_::GameObjectSystemComponent;
		CTransformGUI();
		~CTransformGUI();
		/*アクセサ*/
		//! 位置を取得。Pivotを基準点とする
		const Vector2Di& position()const;
		//! 位置を設定。Pivotを基準点とする
		CTransformGUI& position(const Vector2Di& pos);
		//! 回転度を取得。Pivotを中心とする
		const double rotation_rad()const;
		//! 回転度を設定。Pivotを中心とする
		CTransformGUI& rotation_rad(double rota_rad);
		//! サイズを取得。Pivotを基準点とする
		const Vector2Di& size()const;
		//! サイズを設定。Pivotを基準点とする
		CTransformGUI& size(const Vector2Di& s);
		//! 変形中心位置を取得(0.0-1.0)
		const Vector2Dd& pivot()const;
		//! 変形中心位置を設定(0.0-1.0)
		CTransformGUI& pivot(const Vector2Dd& p);
		//! GUI領域を表す矩形を取得。回転は考慮されない
		//const RectAnglei& rect()const;
		//! アンカー左上の位置を取得。親CTransformGUI中での位置を(0.0-1.0)^2で表す
		//Vector2Dd anchor_min()const;
		//! アンカー左上の位置を設定。親CTransformGUI中での位置を(0.0-1.0)^2で表す
		//CTransformGUI& anchor_min(const Vector2Dd& a);
		//! アンカー右下の位置を取得。親CTransformGUI中での位置を(0.0-1.0)^2で表す
		//Vector2Dd anchor_max()const;
		//! アンカー右下の位置を設定。親CTransformGUI中での位置を(0.0-1.0)^2で表す
		//CTransformGUI& anchor_max(const Vector2Dd& a);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOCオーバーライド
		void OnActivated()override final;
		void OnInactivated()override final;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CTransformGUI);
}