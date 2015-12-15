#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>
#include "Object.h"
#include "RectAngle.h"
#include "ObjectHolderTemplate_WithoutID.h"
#include "GUIManagerConnection.h"

namespace planeta_engine {
	class ScreenDrawerGUI;
	namespace game {
		class GUIComponent;
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
		class GUIObject : public core::Object{
		public:
			GUIObject();
			virtual ~GUIObject() = default;
			/*システム関数*/
			bool Initialize(std::unique_ptr<GUIManagerConnection>&& rc);
			void Update();
			void Draw(ScreenDrawerGUI& drawer);
			void DebugDraw();
			bool KeyInput(ui_object_input_code::type input_code);
			bool PointingCursorPosition(const Vector2D<int>& parent_relative_cursor_position);
			/*ユーザー利用関数*/
			bool Show();
			bool Close();
			void Dispose();
			/*アクセサ*/
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
			std::shared_ptr<C> AddComponent() {
				auto child = std::make_shared<C>();
				component_holder_->resist_object(child);
				return child;
			}
			bool RemoveComponent(const std::shared_ptr<GUIComponent>& c) { return component_holder_->unresist_object(c); }
		private:
			utility::RectAngle<int> rect_angle_;
			bool is_focused_ = false; //フォーカスされているかフラグ
			bool is_position_updated_since_last_draw = true; //前回描画時から位置が更新されたかフラグ
			bool is_size_updated_since_last_draw = true; //前回描画時からサイズが変更されたかフラグ
			std::unique_ptr<utility::ObjectHolderTemplate_WithoutID<GUIComponent>> component_holder_;
			std::unique_ptr<GUIManagerConnection> resister_connection_;
			/*ユーザー定義関数*/
			virtual bool InitializeProc() { return true; }
			virtual void FinalizeProc() {};
			virtual void ShowProc() {};
			virtual void CloseProc() {};
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