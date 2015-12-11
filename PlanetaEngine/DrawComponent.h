#pragma once
#include "GameObjectComponent.h"
#include "WeakPointer.h"
#include "Color.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace system_processes {
		class GameObjectDrawProcess;
	}
	namespace components {
		class DrawComponent : public game::GameObjectComponent {
		public:
			DrawComponent();
			virtual ~DrawComponent() = default;
			/*描画処理*/
			virtual void Draw() = 0;
			/*Accessor*/
			/*描画優先度を取得*/
			int draw_priority()const { return _draw_priority; }
			/*描画優先度を設定*/
			void draw_priority(int priority);
			/*表示位置を取得*/
			const Vector2D<double>& position()const { return _position; }
			/*表示位置を設定*/
			void position(const Vector2D<double>& pos) { _position = pos; }
			/*表示回転度を取得*/
			double rotation_rad()const { return _rotation_rad; }
			/*表示回転度を設定*/
			void rotation_rad(double rota_rad) { _rotation_rad = rota_rad; }
			/*表示拡大度を取得*/
			const Vector2D<double>& scale()const { return _scale; }
			/*表示拡大度を設定*/
			void scale(const Vector2D<double>& s) { _scale = s; }
			/*表示色を取得*/
			core::Color color()const { return _color; }
			/*表示色を設定*/
			void color(const core::Color& c) { _color = c; }
		protected:
			/*描画の中心位置取得(ゲームオブジェクトの形状情報と、表示位置から算出)*/
			Vector2D<double> GetDrawCenterPosition()const;
			/*描画回転度取得(ゲームオブジェクトの回転度と、表示回転度から算出)*/
			double GetDrawRotationRed()const;
			/*描画拡大度取得(ゲームオブジェクトの拡大度と、表示回拡大度から算出)*/
			Vector2D<double> GetDrawScale()const;
		protected:
			/*システム関数(このクラスを継承するクラスで以下の関数をオーバーライドする場合、DrawComponentの同クラスを呼び出さなければならない。)*/
			bool Initialize_()override;
			void Finalize_()override {};
		private:
			int _draw_priority = 0; //描画優先度
			/*表示位置*/
			Vector2D<double> _position;
			/*表示回転度*/
			double _rotation_rad = 0.0;
			/*表示拡大度*/
			Vector2D<double> _scale = Vector2D<double>(1.0, 1.0);
			core::Color _color;
			utility::WeakPointer<system_processes::GameObjectDrawProcess> _game_object_draw_process;
			void _ResistToDrawProcess();
			void _RemoveFromDrawProcess();
			void _UpdatePriority();

			/*システム関数*/
			bool Activated_()override final;
			/*更新処理は行わない*/
			bool NoUpdate_()const override final { return true; }
			bool InActivated_()override final;
		};
	}
}
