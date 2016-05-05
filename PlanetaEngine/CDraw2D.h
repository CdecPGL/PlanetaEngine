#pragma once
#include "GameObjectSystemComponent.h"
#include "WeakPointer.h"
#include "Color.h"
#include "Vector2D.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	class CTransform2D;
	class CDraw2D : public core::GameObjectSystemComponent {
	public:
		CDraw2D();
		virtual ~CDraw2D() = default;
		/*描画処理*/
		void Draw(ScreenDrawer2D& drawer) { DrawProc(drawer); }
		/*Accessor*/
		/*描画優先度を取得*/
		int draw_priority()const { return draw_priority_; }
		/*描画優先度を設定*/
		CDraw2D& draw_priority(int priority);
		/*表示位置を取得*/
		const Vector2Dd& position()const { return position_; }
		/*表示位置を設定*/
		CDraw2D& position(const Vector2Dd& pos) { position_ = pos; return *this; }
		/*表示回転度を取得*/
		double rotation_rad()const { return rotation_rad_; }
		/*表示回転度を設定*/
		CDraw2D& rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
		/*表示拡大度を取得*/
		const Vector2Dd& scale()const { return scale_; }
		/*表示拡大度を設定*/
		CDraw2D& scale(const Vector2Dd& s) { scale_ = s; return *this; }
		/*表示色を取得*/
		planeta_engine::Color color()const { return color_; }
		/*表示色を設定*/
		CDraw2D& color(const planeta_engine::Color& c) { color_ = c; return *this; }
	protected:
		/*描画の中心位置取得(ゲームオブジェクトの形状情報と、表示位置から算出)*/
		Vector2Dd GetDrawCenterPosition()const;
		/*描画回転度取得(ゲームオブジェクトの回転度と、表示回転度から算出)*/
		double GetDrawRotationRed()const;
		/*描画拡大度取得(ゲームオブジェクトの拡大度と、表示回拡大度から算出)*/
		Vector2Dd GetDrawScale()const;

		utility::NonOwingPointer<CTransform2D> transform2d_;

		bool OnInitialized()override;
	private:
		int draw_priority_ = 0; //描画優先度
		/*表示位置*/
		Vector2Dd position_;
		/*表示回転度*/
		double rotation_rad_ = 0.0;
		/*表示拡大度*/
		Vector2Dd scale_ = Vector2Dd(1.0, 1.0);
		planeta_engine::Color color_;
		void RegisterToProcess_();
		void RemoveFromProcess_();
		void UpdatePriority_();
		bool OnActivated()override final;
		bool OnInactivated()override final;
		/*描画処理*/
		virtual void DrawProc(ScreenDrawer2D& drawer) = 0;
	};
}
