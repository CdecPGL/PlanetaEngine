#pragma once
#include "GameObjectSystemComponent.hpp"
#include "WeakPointer.hpp"
#include "Color.hpp"
#include "Vector2D.hpp"

namespace planeta {
	class ScreenDrawer2D;
	class CTransform2D;
	/*! 描画基底コンポーネント*/
	class CDraw2D : public private_::GameObjectSystemComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDraw2D);
	public:
		using Super = private_::GameObjectSystemComponent;
		CDraw2D();
		virtual ~CDraw2D();
		/*描画処理*/
		void Draw(ScreenDrawer2D& drawer);
		/*Accessor*/
		/*! @brief 描画優先度を取得
		
			値が大きいほど手前に表示される*/
		int draw_priority()const { return draw_priority_; }
		/*! @brief 描画優先度を設定
		
			値が大きいほど手前に表示される*/
		CDraw2D& draw_priority(int priority);
		/*! 表示位置を取得(CTransform2Dからの相対値)*/
		const Vector2Dd& relative_position()const { return position_; }
		/*表示位置を設定(CTransform2Dからの相対値)*/
		CDraw2D& relative_position(const Vector2Dd& pos) { position_ = pos; return *this; }
		/*表示回転度を取得(CTransform2Dからの相対値)*/
		double relative_rotation_rad()const { return rotation_rad_; }
		/*表示回転度を設定(CTransform2Dからの相対値)*/
		CDraw2D& relative_rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
		/*! 表示拡大度を取得(CTransform2Dからの相対値)*/
		const Vector2Dd& relative_scale()const { return scale_; }
		/*! 表示拡大度を設定(CTransform2Dからの相対値)*/
		CDraw2D& relative_scale(const Vector2Dd& s) { scale_ = s; return *this; }
		/*! 表示色を取得*/
		planeta::Color color()const { return color_; }
		/*! 表示色を設定*/
		CDraw2D& color(const planeta::Color& c) { color_ = c; return *this; }
	protected:
		/*描画の中心位置取得(ゲームオブジェクトの形状情報と、表示位置から算出)*/
		Vector2Dd GetDrawCenterPosition()const;
		/*描画回転度取得(ゲームオブジェクトの回転度と、表示回転度から算出)*/
		double GetDrawRotationRed()const;
		/*描画拡大度取得(ゲームオブジェクトの拡大度と、表示回拡大度から算出)*/
		Vector2Dd GetDrawScale()const;

		NonOwingPointer<CTransform2D> transform2d_;

		virtual bool GetOtherComponentsProc(const GOComponentGetter& com_getter)override;

		void OnInitialized()override;
		void OnFinalized()noexcept override;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		int draw_priority_ = 0; //描画優先度
		/*表示位置*/
		Vector2Dd position_;
		/*表示回転度*/
		double rotation_rad_ = 0.0;
		/*表示拡大度*/
		Vector2Dd scale_ = Vector2Dd(1.0, 1.0);
		planeta::Color color_;
		void UpdatePriority_();
		void OnActivated()override final;
		void OnInactivated()override final;
		/*描画処理*/
		virtual void DrawProc(ScreenDrawer2D& drawer) = 0;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDraw2D);
}
