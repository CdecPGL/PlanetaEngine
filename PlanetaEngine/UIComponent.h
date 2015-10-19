#pragma once

#include <unordered_map>
#include <memory>
#include <array>
#include "Object.h"
#include "RectAngle.h"
#include "ObjectHolderTemplate_WithoutID.h"

namespace planeta_engine {
	namespace game {
		namespace anchor_style {
			using type = unsigned int;
			constexpr type None = 0u;
			constexpr type Top = 1u;
			constexpr type Bottom = 2u;
			constexpr type Left = 4u;
			constexpr type Right = 8u;
			constexpr type All = ~0u;
		}
		class UIComponent : public core::Object{
		public:
			UIComponent();
			virtual ~UIComponent() = default;
			void Update() { UpdateProc(); UpdateChildren(); };
			void Draw(const utility::RectAngle<int>& holder_draw_area);

			const utility::RectAngle<int>& rect()const { return rect_angle_; }
			void rect(const utility::RectAngle<int>& r) { rect_angle_ = r; OnRectAngleUpdated(); }
			const Vector2D<int> position()const { return rect_angle_.position; }
			void position(const Vector2D<int>& p) { rect_angle_.position = p; OnPositionUpdated(); OnRectAngleUpdated(); }
			const Vector2D<int> size()const { return rect_angle_.size; }
			void size(const Vector2D<int>& s) { rect_angle_.size = s; OnSizeUpdated(); OnRectAngleUpdated(); }
			const anchor_style::type anchor()const { return anchor_; }
			void anchor(anchor_style::type a) { anchor_ = a; }

			std::shared_ptr<UIComponent> AddChild(){}
			bool RemoveChild(const std::shared_ptr<UIComponent>& c){}
		private:
			std::unique_ptr<utility::ObjectHolderTemplate_WithoutID<UIComponent>> children_holder_;
			/*更新処理*/
			virtual void UpdateProc() = 0;
			void UpdateChildren();
			void DrawChildren(const utility::RectAngle<int>& my_draw_area);
			/*描画処理。描画は引数として渡される範囲内に行う*/
			virtual void DrawProc(const utility::RectAngle<int>& my_draw_area) = 0;
			utility::RectAngle<int> rect_angle_;			
			anchor_style::type anchor_; //アンカー
			/*各種イベント*/
			void UpadteRectAngle(const utility::RectAngle<int>& rect);
			void UpdatePosition(const Vector2D<int>& position);
			void UpdateSize(const Vector2D<int>& size);
			virtual void OnRectAngleUpdated(const utility::RectAngle<int>& rect) {};
			virtual void OnPositionUpdated(const Vector2D<int>& position) {};
			virtual void OnSizeUpdated(const Vector2D<int>& size) {};
		};
	}
}
