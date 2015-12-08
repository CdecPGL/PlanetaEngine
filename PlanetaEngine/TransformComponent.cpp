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
			//速度を適用
			global_position_ += _velocity;
			global_rotation_rad_ += _rotation_velocity_rad;
			ConvertGlobalToLocal();
		}

		void TransformComponent::ConvertLocalToGlobal()
		{
			if (parent()) {
				//親の位置から自分の位置を求める
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
			//自分と親の現在の絶対座標からローカル座標を求める
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
			children_.emplace(child.get(), child); //子リストに追加
			if (child->parent_) {
				auto it = child->parent_->children_.find(child.get());
				assert(it != child->parent_->children_.end()); //前の親に子がない
				child->parent_->children_.erase(it); //子の親の前のから子を除去
			}
			child->parent_ = std::static_pointer_cast<TransformComponent>(this_shared()); //自分を現在の親に設定
			child->ConvertGlobalToLocal();
		}

		void TransformComponent::RemoveChild(const utility::WeakPointer<TransformComponent>& c)
		{
			utility::WeakPointer<TransformComponent> child = c;
			auto it = children_.find(child.get());
			if (it == children_.end()) { return; }
			children_.erase(it); //子リストから除去
			child->parent_ = root_transform(); //元子の親をRootに設定
			child->parent_->children_.emplace(child.get(), child); //Rootに元子を追加
			child->ConvertGlobalToLocal();
		}

		void TransformComponent::parent(const utility::WeakPointer<TransformComponent>& p)
		{
			auto this_transform_shared = std::static_pointer_cast<TransformComponent>(this_shared());
			if (parent_) {
				auto it = parent_->children_.find(this);
				assert(it != parent_->children_.end()); //前の親に子がない
				parent_->children_.erase(it); //元親から子を除去
			}
			parent_ = p; //新しい親を設定
			parent_->children_.emplace(this, this_transform_shared); //新しい親の子に自分を追加
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