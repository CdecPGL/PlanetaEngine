#pragma once

#include <unordered_map>
#include <memory>
#include <bitset>
#include <array>
#include "Object.h"
#include "RectAngle.h"
#include "ObjectHolderTemplate_WithoutID.h"
#include "NonCopyable.h"

namespace planeta_engine {
	class ScreenDrawerGUI;
	namespace game {
		class GUIComponent : public core::Object,private utility::NonCopyable<GUIComponent>{
		public:
			GUIComponent();
			virtual ~GUIComponent() = default;
			/*�V�X�e���֐�*/
			struct UpdateInfo {
				bool is_position_updated = false;
				bool is_size_updated = false;
				bool is_edge_fix_updated = false;
			};
			struct DrawData {
				utility::RectAngle<int> draw_area;
				UpdateInfo update_info;
			};
			bool Initialize();
			void Finalize();
			void Update();
			void Draw(const DrawData& parent_draw_data, ScreenDrawerGUI& drawer);
			void DebugDraw(const utility::RectAngle<int>& parent_draw_area);

			enum class edge { Right = 0, Left = 1, Top = 2, Bottom = 3 };
			/*���J�֐�*/
			/**
			* @brief �ӂ��Œ肷��
			* @param (e) �Ώۂ̕�
			* @param (padding) �]��
			*/
			void FixEdge(edge e, int padding);
			/**
			* @brief ���ׂĂ̕ӂ��Œ肷��
			* @param (left_padding) ���ӂ̗]��
			* @param (right_padding) ���ӂ̗]��
			* @param (top_padding) ���ӂ̗]��
			* @param (bottom_padding) ���ӂ̗]��
			*/
			void FixAllEdge(int left_padding, int right_padding, int top_padding, int bottom_padding);
			/**
			* @brief �ӂ̌Œ������
			* @param (e) �Ώۂ̕�
			*/
			void UnfixEdge(edge e);
			/*�A�N�Z�T*/
			const utility::RectAngle<int>& rect()const { return rect_angle_; }
			void rect(const utility::RectAngle<int>& r) { rect_angle_ = r; is_position_updated_since_last_draw = true; is_size_updated_since_last_draw = true; }
			const Vector2D<int> position()const { return rect_angle_.position; }
			void position(const Vector2D<int>& p) { rect_angle_.position = p; is_position_updated_since_last_draw = true; }
			const Vector2D<int> size()const { return rect_angle_.size; }
			void size(const Vector2D<int>& s) { rect_angle_.size = s; is_size_updated_since_last_draw = true; }

			template<class C>
			std::shared_ptr<C> AddChild() {
				auto child = std::make_shared<C>();
				children_holder_->resist_object(child);
				return child;
			}
			bool RemoveChild(const std::shared_ptr<GUIComponent>& c) { return children_holder_->unresist_object(c); }
		private:

			std::unique_ptr<utility::ObjectHolderTemplate_WithoutID<GUIComponent>> children_holder_;
			bool is_position_updated_since_last_draw = true; //�O��`�掞����ʒu���X�V���ꂽ���t���O
			bool is_size_updated_since_last_draw = true; //�O��`�掞����T�C�Y���ύX���ꂽ���t���O
			bool is_edge_fix_updated_since_last_draw = true; //�O��`�掞����ӌŒ肪�ύX���ꂽ���t���O
			utility::RectAngle<int> relative_draw_area_buffer_; //�`��G���A
			void CalculateDrawRectAndUpdateInfo(const DrawData& parent_draw_data, UpdateInfo* my_update_info); //�`��G���A�ƍX�V�����v�Z����
			 /*�ʒu���*/
			utility::RectAngle<int> rect_angle_;
			/*�A���J�[*/
			std::array<int, 4> edge_fix_padding_; //�ӌŒ�̗]��
			std::bitset<4> edge_fix_bit_set_ = 0; //�ӌŒ�̗L����\���r�b�g�z��

			/*�q������*/
			void UpdateChildren();
			void DrawChildren(const DrawData& my_draw_data, ScreenDrawerGUI& drawer);

			/*�I�[�o�[���C�h�\�֐��Q*/
			/*�X�V����*/
			virtual void UpdateProc() {};
			/*�O��`�掞����ʒu���X�V���ꂽ�Ƃ��̏���(�`��O�ɌĂ΂��)*/
			virtual void PositionUpdatedProc(const Vector2D<int>& position) {};
			/*�O��`�掞����T�C�Y���X�V���ꂽ�Ƃ��̏���(�`��O�ɌĂ΂��)*/
			virtual void SizeUpdatedProc(const Vector2D<int>& size) {};
			/*�`�揈���B�`��͈����Ƃ��ēn�����͈͓��ɍs��*/
			virtual void DrawProc(const utility::RectAngle<int>& my_draw_area,ScreenDrawerGUI& drawer) = 0;
			/*����������*/
			virtual bool InitializeProc() { return true; };
			/*�I������*/
			virtual void FinalizeProc() {};
		};
	}
}
