#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>
#include "Object.h"
#include "RectAngle.h"
#include "ObjectHolderTemplate_WithoutID.h"

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
		class UIObject : public core::Object{
		public:
			UIObject();
			virtual ~UIObject() = default;
			bool Initialize() { return InitializeProc(); }
			void Finalize() { return FInalizeProc(); }
			void Update();
			void Draw(const utility::RectAngle<int>& parent_area);
			bool KeyInput(ui_object_input_code::type input_code);
			bool PointingCursorPosition(const Vector2D<int>& parent_relative_cursor_position);

			void Show();
			void Close();
			
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
		private:
			utility::RectAngle<int> rect_angle_;
			bool is_focused_ = false; //フォーカスされているかフラグ
			std::unique_ptr<utility::ObjectHolderTemplate_WithoutID<UIComponent>> component_holder_;
			virtual bool InitializeProc() {}
			virtual void FInalizeProc() {};
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