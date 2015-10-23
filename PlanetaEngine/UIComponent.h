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
			/*�V�X�e���֐�*/
			struct DrawData {
				utility::RectAngle<int> draw_area;
				bool is_position_updated = false;
				bool is_size_updated = false;
			};
			void Update();
			void Draw(const DrawData& parent_draw_data);

			/*�A�N�Z�T*/
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
			bool is_position_updated_since_last_draw = true; //�O��`�掞����ʒu���X�V���ꂽ���t���O
			bool is_size_updated_since_last_draw = true; //�O��`�掞����T�C�Y���ύX���ꂽ���t���O
			 /*�ʒu���*/
			utility::RectAngle<int> rect_angle_;
			/*�A���J�[*/
			anchor_style::type anchor_;

			/*�q������*/
			void UpdateChildren();
			void DrawChildren(const DrawData& my_draw_data);

			/*�I�[�o�[���C�h�\�֐��Q*/
			/*�X�V����*/
			virtual void UpdateProc() {};
			/*�O��`�掞����ʒu���X�V���ꂽ�Ƃ��̏���(�`��O�ɌĂ΂��)*/
			virtual void PositionUpdatedProc(const Vector2D<int>& position) {};
			/*�O��`�掞����T�C�Y���X�V���ꂽ�Ƃ��̏���(�`��O�ɌĂ΂��)*/
			virtual void SizeUpdatedProc(const Vector2D<int>& size) {};
			/*�`�揈���B�`��͈����Ƃ��ēn�����͈͓��ɍs��*/
			virtual void DrawProc(const utility::RectAngle<int>& my_draw_area) = 0;
		};
	}
}
