#pragma once

#include <unordered_map>
#include <memory>
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
			/*システム関数*/
			struct DrawData {
				utility::RectAngle<int> draw_area;
				bool is_position_updated = false;
				bool is_size_updated = false;
			};
			void Update();
			void Draw(const DrawData& parent_draw_data);

			/*アクセサ*/
			const utility::RectAngle<int>& rect()const { return rect_angle_; }
			void rect(const utility::RectAngle<int>& r) { rect_angle_ = r; is_position_updated_since_last_draw = true; is_size_updated_since_last_draw = true; }
			const Vector2D<int> position()const { return rect_angle_.position; }
			void position(const Vector2D<int>& p) { rect_angle_.position = p; is_position_updated_since_last_draw = true; }
			const Vector2D<int> size()const { return rect_angle_.size; }
			void size(const Vector2D<int>& s) { rect_angle_.size = s; is_size_updated_since_last_draw = true; }
			const anchor_style::type anchor()const { return anchor_; }
			void anchor(anchor_style::type a) { anchor_ = a; }

			template<class C>
			std::shared_ptr<C> AddChild() {
				auto child = std::make_shared<C>();
				children_holder_->resist_object(child);
				return child;
			}
			bool RemoveChild(const std::shared_ptr<UIComponent>& c) { return children_holder_->unresist_object(c); }
		private:
			std::unique_ptr<utility::ObjectHolderTemplate_WithoutID<UIComponent>> children_holder_;
			bool is_position_updated_since_last_draw = true; //前回描画時から位置が更新されたかフラグ
			bool is_size_updated_since_last_draw = true; //前回描画時からサイズが変更されたかフラグ
			 /*位置情報*/
			utility::RectAngle<int> rect_angle_;
			/*アンカー*/
			anchor_style::type anchor_;

			/*子供処理*/
			void UpdateChildren();
			void DrawChildren(const DrawData& my_draw_data);

			/*オーバーライド可能関数群*/
			/*更新処理*/
			virtual void UpdateProc() {};
			/*前回描画時から位置が更新されたときの処理(描画前に呼ばれる)*/
			virtual void PositionUpdatedProc(const Vector2D<int>& position) {};
			/*前回描画時からサイズが更新されたときの処理(描画前に呼ばれる)*/
			virtual void SizeUpdatedProc(const Vector2D<int>& size) {};
			/*描画処理。描画は引数として渡される範囲内に行う*/
			virtual void DrawProc(const utility::RectAngle<int>& my_draw_area) = 0;
		};
	}
}
