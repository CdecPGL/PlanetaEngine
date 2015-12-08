#include "TransformComponent.h"
#include "Matrix.h"
#include <cassert>

namespace planeta_engine {
	namespace components {

		TransformComponent::TransformComponent() :local_rotation_rad_(0.0), local_scale_(1.0, 1.0)
		{

		}

		void TransformComponent::ApplyVelocity()
		{
			//���x��K�p
			global_position_ += _velocity;
			global_rotation_rad_ += _rotation_velocity_rad;
			ConvertGlobalToLocal();
		}

		void TransformComponent::ConvertLocalToGlobal()
		{
			if (parent()) {
				//�e�̈ʒu���玩���̈ʒu�����߂�
				auto&  parent_transform = *parent();
				if (position_parent_dependence_) {
					global_position_ = parent_transform.global_position() + math::RotationalTransformation(parent_transform.global_rotation_rad(), math::Vector2Dd(local_position_.x * parent_transform.global_scale().x, local_position_.y * parent_transform.global_scale().y));
				}
				else {
					global_position_ = local_position_;
				}
				if (scale_parent_dependence_) {
					global_scale_ = math::Vector2Dd(parent_transform.global_scale().x * local_scale_.x, parent_transform.global_scale().y * local_scale_.y);
				}
				else {
					global_scale_ = local_scale_;
				}
				if (rotation_parent_dependence_) {
					global_rotation_rad_ = parent_transform.global_rotation_rad() + local_rotation_rad_;
				}
				else {
					global_rotation_rad_ = local_rotation_rad_;
				}
			}
			else {
				global_position_ = local_position_;
				global_scale_ = local_scale_;
				global_rotation_rad_ = local_rotation_rad_;
			}
			ConvertLocalToGlobalChildren();
		}

		void TransformComponent::ConvertGlobalToLocal()
		{
			//�����Ɛe�̌��݂̐�΍��W���烍�[�J�����W�����߂�
			if (parent()) {
				auto&  parent_transform = *parent();
				if (position_parent_dependence_) {
					auto no_rota_local_pos = math::RotationalTransformation(-parent_transform.global_rotation_rad(), global_position_ - parent_transform.global_position());
					local_position_.Set(no_rota_local_pos.x / parent_transform.global_scale().x, no_rota_local_pos.y / parent_transform.global_scale().y);
				}
				else {
					local_position_ = global_position_;
				}
				if (scale_parent_dependence_) {
					local_scale_ = math::Vector2Dd(global_scale().x / parent_transform.global_scale().x, global_scale().y / parent_transform.global_scale().y);
				}
				else {
					local_scale_ = global_scale_;
				}
				if (rotation_parent_dependence_) {
					local_rotation_rad_ = global_rotation_rad() - parent_transform.global_rotation_rad();
				}
				else {
					local_rotation_rad_ = global_rotation_rad_;
				}
			}
			else {
				local_position_ = global_position_;
				local_scale_ = global_scale_;
				local_rotation_rad_ = global_rotation_rad_;
			}
			ConvertLocalToGlobalChildren();
		}

		void TransformComponent::AddChild(const utility::WeakPointer<TransformComponent>& c)
		{
			utility::WeakPointer<TransformComponent> child = c;
			children_.emplace(child.get(), child); //�q���X�g�ɒǉ�
			if (child->parent_) {
				auto it = child->parent_->children_.find(child.get());
				assert(it != child->parent_->children_.end()); //�O�̐e�Ɏq���Ȃ�
				child->parent_->children_.erase(it); //�q�̐e�̑O�̂���q������
			}
			child->parent_ = std::static_pointer_cast<TransformComponent>(this_shared()); //���������݂̐e�ɐݒ�
			child->ConvertGlobalToLocal();
		}

		void TransformComponent::RemoveChild(const utility::WeakPointer<TransformComponent>& c)
		{
			utility::WeakPointer<TransformComponent> child = c;
			auto it = children_.find(child.get());
			if (it == children_.end()) { return; }
			children_.erase(it); //�q���X�g���珜��
			child->parent_ = root_transform(); //���q�̐e��Root�ɐݒ�
			child->parent_->children_.emplace(child.get(), child); //Root�Ɍ��q��ǉ�
			child->ConvertGlobalToLocal();
		}

		void TransformComponent::parent(const utility::WeakPointer<TransformComponent>& p)
		{
			auto this_transform_shared = std::static_pointer_cast<TransformComponent>(this_shared());
			if (parent_) {
				auto it = parent_->children_.find(this);
				assert(it != parent_->children_.end()); //�O�̐e�Ɏq���Ȃ�
				parent_->children_.erase(it); //���e����q������
			}
			parent_ = p; //�V�����e��ݒ�
			parent_->children_.emplace(this, this_transform_shared); //�V�����e�̎q�Ɏ�����ǉ�
			ConvertGlobalToLocal();
		}

		void TransformComponent::ApplyVelocityRecursively()
		{
			ApplyVelocity();
			for (auto it = children_.begin(); it != children_.end();) {
				if (it->second) {
					it->second->ApplyVelocityRecursively();
					++it;
				}
				else {
					children_.erase(it++);
				}
			}
		}

		void TransformComponent::ConvertLocalToGlobalChildren()
		{
			for (auto it = children_.begin(); it != children_.end();) {
				if (it->second) {
					it->second->ConvertLocalToGlobal();
					++it;
				}
				else {
					children_.erase(it++);
				}
			}
		}

		utility::WeakPointer<TransformComponent> TransformComponent::root_transform()
		{
			utility::WeakPointer<TransformComponent> root = std::static_pointer_cast<TransformComponent>(this_shared());
			while (root->parent_) { root = root->parent_; }
			return root;
		}

	}
}