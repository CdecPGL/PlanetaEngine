#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>
#include "Object.h"
#include "RectAngle.h"
#include "ObjectHolderTemplate_WithoutID.h"
#include "UIObjectResisterConnection.h"

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
			/*�V�X�e���֐�*/
			bool Initialize(std::unique_ptr<UIObjectResisterConnection>&& rc);
			void Update();
			void Draw();
			void DebugDraw();
			bool KeyInput(ui_object_input_code::type input_code);
			bool PointingCursorPosition(const math::Vector2Di& parent_relative_cursor_position);
			/*���[�U�[���p�֐�*/
			bool Show();
			bool Close();
			void Dispose();
			/*�A�N�Z�T*/
			const math::Vector2Di position()const { return rect_angle_.position; }
			void position(const math::Vector2Di& p) { rect_angle_.position = p; PositionChangedProc(); }
			const math::Vector2Di size()const { return rect_angle_.size; }
			void size(const math::Vector2Di& s) { rect_angle_.size = s; SizeChangedProc(); }
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
			bool RemoveComponent(const std::shared_ptr<UIComponent>& c) { return component_holder_->unresist_object(c); }
		private:
			utility::RectAngle<int> rect_angle_;
			bool is_focused_ = false; //�t�H�[�J�X����Ă��邩�t���O
			bool is_position_updated_since_last_draw = true; //�O��`�掞����ʒu���X�V���ꂽ���t���O
			bool is_size_updated_since_last_draw = true; //�O��`�掞����T�C�Y���ύX���ꂽ���t���O
			std::unique_ptr<utility::ObjectHolderTemplate_WithoutID<UIComponent>> component_holder_;
			std::unique_ptr<UIObjectResisterConnection> resister_connection_;
			/*���[�U�[��`�֐�*/
			virtual bool InitializeProc() { return true; }
			virtual void FinalizeProc() {};
			virtual void ShowProc() {};
			virtual void CloseProc() {};
			/**�L�[�̓��͏���
			@param ���̓R�[�h
			@return �q�ɓ`������̓R�[�h
			*/
			virtual ui_object_input_code::type KeyInputProc(ui_object_input_code::type input_code) { return input_code; }
			//�J�[�\���̏Փ˔��菈��(�Փ˂�������Ԃ�)
			virtual bool DetectCursorPointedProc(const math::Vector2Di& my_relative_cursor_position) { return false; }
			/*�ʒu���ύX���ꂽ�Ƃ��̏���*/
			virtual void PositionChangedProc() {}
			/*�T�C�Y���ύX���ꂽ�Ƃ��̏���*/
			virtual void SizeChangedProc() {}
			/*�t�H�[�J�X���ꂽ�Ƃ��̏���*/
			virtual void FocusedProc() {}
			/*�t�H�[�J�X���O�ꂽ���̏���*/
			virtual void BlurredProc() {}
		};
	}
}