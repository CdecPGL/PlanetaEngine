#pragma once

#include "Object.h"
#include "RectAngle.h"

namespace planeta_engine {
	namespace game {
		class UIComponent : public core::Object {
		public:
			virtual ~UIComponent() = default;
			void Update() { UpdateProc(); };
			void Draw(const utility::RectAngle<int>& holder_draw_area);

			const utility::RectAngle<int>& rect()const { return rect_angle_; }
			void rect(const utility::RectAngle<int>& r) { rect_angle_ = r; OnRectAngleUpdated(); }
			const Vector2D<int> position()const { return rect_angle_.position; }
			void position(const Vector2D<int>& p) { rect_angle_.position = p; OnPositionUpdated(); OnRectAngleUpdated(); }
			const Vector2D<int> size()const { return rect_angle_.size; }
			void size(const Vector2D<int>& s) { rect_angle_.size = s; OnSizeUpdated(); OnRectAngleUpdated(); }
		private:
			/*更新処理*/
			virtual void UpdateProc() = 0;
			/*描画処理。描画は引数として渡される範囲内に行う*/
			virtual void DrawProc(const utility::RectAngle<int>& my_draw_area) = 0;
			utility::RectAngle<int> rect_angle_;
			/*各種イベント*/
			virtual void OnRectAngleUpdated() {};
			virtual void OnPositionUpdated() {};
			virtual void OnSizeUpdated() {};
		};
	}
}
