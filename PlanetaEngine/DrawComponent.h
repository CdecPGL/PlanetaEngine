#pragma once
#include "GameObjectSpecialComponent.h"
#include "WeakPointer.h"
#include "Color.h"
#include "Vector2D.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	namespace system_processes {
		class GameObjectDrawProcessCore;
	}
	namespace core {
//		class GameObjectDrawComponentProcessRegistrator;
	}
	namespace components {
		class DrawComponent : public core::GameObjectSpecialComponent {
		public:
			DrawComponent();
			virtual ~DrawComponent() = default;
			/*描画処理*/
			void Draw(ScreenDrawer2D& drawer) { DrawProc(drawer); }
			/*Accessor*/
			/*描画優先度を取得*/
			int draw_priority()const { return draw_priority_; }
			/*描画優先度を設定*/
			DrawComponent& draw_priority(int priority);
			/*表示位置を取得*/
			const Vector2D<double>& position()const { return position_; }
			/*表示位置を設定*/
			DrawComponent& position(const Vector2D<double>& pos) { position_ = pos; return *this; }
			/*表示回転度を取得*/
			double rotation_rad()const { return rotation_rad_; }
			/*表示回転度を設定*/
			DrawComponent& rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
			/*表示拡大度を取得*/
			const Vector2D<double>& scale()const { return scale_; }
			/*表示拡大度を設定*/
			DrawComponent& scale(const Vector2D<double>& s) { scale_ = s; return *this; }
			/*表示色を取得*/
			planeta_engine::Color color()const { return color_; }
			/*表示色を設定*/
			DrawComponent& color(const planeta_engine::Color& c) { color_ = c; return *this; }
		protected:
			/*描画の中心位置取得(ゲームオブジェクトの形状情報と、表示位置から算出)*/
			Vector2D<double> GetDrawCenterPosition()const;
			/*描画回転度取得(ゲームオブジェクトの回転度と、表示回転度から算出)*/
			double GetDrawRotationRed()const;
			/*描画拡大度取得(ゲームオブジェクトの拡大度と、表示回拡大度から算出)*/
			Vector2D<double> GetDrawScale()const;
		private:
			int draw_priority_ = 0; //描画優先度
			/*表示位置*/
			Vector2D<double> position_;
			/*表示回転度*/
			double rotation_rad_ = 0.0;
			/*表示拡大度*/
			Vector2D<double> scale_ = Vector2D<double>(1.0, 1.0);
			planeta_engine::Color color_;
//			std::shared_ptr<core::GameObjectDrawComponentProcessRegistrator> draw_component_registrator_;
			utility::WeakPointer<system_processes::GameObjectDrawProcessCore> draw_component_registrator_;
			void RegisterToProcess_();
			void RemoveFromProcess_();
			void UpdatePriority_();
			/*特殊セットアップ*/
			bool SpecialSetUp(const core::SceneDataForGameObject& setup_data)override final;
			/*更新処理は行わない*/
			bool is_no_update()const override final { return true; }
			bool OnActivated()override final;
			bool OnInactivated()override final;
			/*描画処理*/
			virtual void DrawProc(ScreenDrawer2D& drawer) = 0;
		};
	}
}
