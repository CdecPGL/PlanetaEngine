#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>
#include "Object.h"
#include "RectAngle.h"
#include "IUIComponentContainer.h"

namespace planeta_engine {
	namespace game {
		class UIComponent;
		namespace ui_object_input_code {
			using type = uint32_t;
			constexpr type UP = 1,
				DOWN = 2,
				LEFT = 4,
				RIGHT = 8,
				SELECT = 16,
				CANCEL = 32,
				Error = 1ul << 31;
		}
		class UIObject : public core::Object,protected IUIComponentContainer{
		public:
			virtual ~UIObject() = default;
			void Update();
			void Draw(const utility::RectAngle<int>& parent_area);
			bool KeyInput(ui_object_input_code::type input_code);
			bool PointingCursorPosition(const Vector2D<int>& parent_relative_cursor_position);
			
			const Vector2D<int> position()const { return rect_angle_.position; }
			void position(const Vector2D<int>& p) { rect_angle_.position = p; PositionChangedProc(); }
			const Vector2D<int> size()const { return rect_angle_.size; }
			void size(const Vector2D<int>& s) { rect_angle_.size = s; SizeChangedProc(); }
			bool is_focused()const { return is_focused_; }
			void is_focused(bool f) {
				if (f == is_focused_) { return; }
				else {
					if (f) { is_focused_ = true; FocusedProc(); }
					else { is_focused_ = false; BlurredProc(); }
				}
			}
		protected:
			template<class C>
			std::shared_ptr<UIComponent> AddComponent() {
				std::shared_ptr<UIComponent> component = std::make_shared<C>();
				components_.push_back(component);
				return component;
			}
		private:
			std::vector<std::shared_ptr<UIObject>> components_;
			utility::RectAngle<int> rect_angle_;
			bool is_focused_ = false; //フォーカスされているかフラグ
			//更新処理
			void UpdateComponents();
			//描画処理
			void DrawComponents(const utility::RectAngle<int>& draw_area);
			/**キーの入力処理
			@param 入力コード
			@return 子に伝える入力コード
			*/
			virtual ui_object_input_code::type KeyInputProc(ui_object_input_code::type input_code) { return input_code; }
			//カーソルの衝突判定処理(衝突したかを返す)
			virtual bool DetectCursorPointedProc(const Vector2D<int>& my_relative_cursor_position) { return false; }
			/*位置が変更されたときの処理*/
			virtual void PositionChangedProc() {}
			/*サイズが変更されたときの処理*/
			virtual void SizeChangedProc() {}
			/*フォーカスされたときの処理*/
			virtual void FocusedProc() {}
			/*フォーカスが外れた時の処理*/
			virtual void BlurredProc() {}
		};
	}
}