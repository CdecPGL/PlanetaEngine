#pragma once

#include "GameObjectSystemComponent.h"
#include "Vector2D.h"

namespace planeta {
	class CTransformGUI final : public private_::GameObjectSystemComponent{
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CTransformGUI);
	public:
		using Super = private_::GameObjectSystemComponent;
		CTransformGUI();
		~CTransformGUI();
		/*アクセサ*/
		//! 位置を取得
		const Vector2Di& position()const;
		//! 位置を設定
		CTransformGUI& position(const Vector2Di& pos);
		//! 回転度を取得
		const double rotation_rad()const;
		//! 回転度を設定
		CTransformGUI&  rotation_rad(double rota_rad);
		//! サイズを設定
		CTransformGUI& size(const Vector2Di& s);
		//! サイズを取得
		const Vector2Di& size()const;
		//! 中心位置を設定(0.0-1.0)
		CTransformGUI& pivot(const Vector2Dd& p);
		//! 中心位置を取得(0.0-1.0)
		const Vector2Dd& pivot()const;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOCオーバーライド
		bool OnActivated()override final;
		bool OnInactivated()override final;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CTransformGUI);
}