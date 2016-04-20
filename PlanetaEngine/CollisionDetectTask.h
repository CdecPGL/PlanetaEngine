#pragma once
#include "Task.h"
#include <memory>
#include <unordered_map>
#include <list>

namespace planeta_engine {
	namespace components {
		class ColliderComponent;
	}
	namespace core {
		class CollisionGroupMatrix;
	}
	namespace event_arguments {
		class CollisionEventArgument;
		class CollisionWithGroundEventArgument;
	}
	namespace system_processes {
		class CollisionDetectTask final: public game::Task
		{
		public:
			using Task::Task;
			~CollisionDetectTask();
			void SetCollisionGroupMatrix(const std::shared_ptr<const core::CollisionGroupMatrix>& col_matrix);
			bool Resist(const std::shared_ptr<components::ColliderComponent>& col_com);
			bool Remove(const components::ColliderComponent* col_com_ptr);
			bool ChangeCollisionGroup(const components::ColliderComponent* col_com_ptr,const std::string& group_name);
			bool ChangeCollisionWithGroundFlag(const components::ColliderComponent* col_com_ptr, bool flag);
		private:
			void Update()override final;
			void RemoveAll();
			using CollisionGroupType = std::list<components::ColliderComponent*>;
			using CollisionWithGroundListType = std::list<components::ColliderComponent*>;
			struct ColliderComponentResistData_ {
				std::shared_ptr<components::ColliderComponent> pointer; //�R���C�_�[�̃|�C���^�[
				std::unordered_map<std::string, CollisionGroupType>::iterator group_iterator_at_collision_groups; //�R���W�����O���[�v�̃O���[�v���X�g���ł̃C�e���[�^
				CollisionGroupType::iterator iterator_at_collision_group; //�R���W�����O���[�v���ł̃C�e���[�^
				bool collision_with_ground_flag; //�n�`�Ƃ̏Փ˃t���O
				CollisionWithGroundListType::iterator iterator_at_collision_with_ground_list; //�n�`�ƏՓ˂���ꍇ�́A�n�`�Փ˃��X�g���ł̃C�e���[�^
			};
			std::unordered_map<components::ColliderComponent*, ColliderComponentResistData_> collider_resist_data_map_; //�R���C�_�[�̓o�^��񃊃X�g
			using CollisionGroupListType = std::unordered_map<std::string, CollisionGroupType>;
			CollisionGroupListType collision_groupes_; //�Փ˃O���[�v
			CollisionWithGroundListType collision_with_ground_list_; //�n�`�ƏՓ˂���R���C�_�[�̃��X�g

			std::vector<std::pair<CollisionGroupListType::iterator, CollisionGroupListType::iterator>> collide_group_pair_list_; //�Փ˂���O���[�v�y�A�̃��X�g

			using CollisionColliderEventHolderType = std::vector<std::pair<components::ColliderComponent*, event_arguments::CollisionEventArgument>>;
			using CollisionGroundEventHolderType = std::vector<std::pair<components::ColliderComponent*, event_arguments::CollisionWithGroundEventArgument>>;
			void ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionColliderEventHolderType& collision_event_holder)const; //�Q�̃O���[�v�Ԃł̏Փ˔���
			void ProcessCollisionInAGroup(CollisionGroupType& group, CollisionColliderEventHolderType& collision_event_holder)const; //�O���[�v���ł̏Փ˔���
			void ProcessCollisionWithGround(CollisionGroundEventHolderType& collision_event_holder)const;
		};
	}
}
