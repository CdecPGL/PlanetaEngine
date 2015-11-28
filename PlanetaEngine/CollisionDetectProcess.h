#pragma once
#include "GameProcess.h"
#include <memory>
#include <unordered_map>
#include <forward_list>

namespace planeta_engine {
	namespace components {
		class ColliderComponent;
	}
	namespace core {
		class CollisionGroupMatrix;
	}
	namespace system_processes {
		class CollisionDetectProcess : public game::GameProcess
		{
		public:
			GameProcess::GameProcess;
			~CollisionDetectProcess();
			void SetCollisionGroupMatrix(const std::shared_ptr<const core::CollisionGroupMatrix>& col_matrix);
			bool Resist(const std::shared_ptr<components::ColliderComponent>& col_com);
			bool Remove(const components::ColliderComponent* col_com_ptr);
			bool ChangeCollisionGroup(const components::ColliderComponent* col_com_ptr,const std::string& group_name);
			bool ChangeCollisionWithGroundFlag(const components::ColliderComponent* col_com_ptr, bool flag);
		private:
			void Update()override final;
			void RemoveAll();
			using CollisionGroupType = std::forward_list<components::ColliderComponent*>;
			using CollisionWithGroundListType = std::forward_list<components::ColliderComponent*>;
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

			struct Request_ {
				enum class Kind { Add, Remove, ChangeCollisionGroup, ChangeCollisionWithGroundFlag };
				Request_(Kind kind):detail(kind){}
				std::unordered_map<components::ColliderComponent*, ColliderComponentResistData_>::iterator resist_data_iterator;
				union Detail {
					explicit Detail(Kind k);
					~Detail();
					Detail(const Detail& d);
					struct AddDetail{ Kind kind; std::shared_ptr<components::ColliderComponent> col_com; } add_detail;
					struct RemoveDetail{ Kind kind; } remove_detail;
					struct ChangeCollisionGroupDetail{ Kind kind;  CollisionGroupListType::iterator new_collision_group_iterator; } change_collision_group_detail;
					struct ChangeCollisionWithGroundFlagDetail{ Kind kind;  bool new_collision_with_ground_flag; } change_collision_with_ground_flag_detail;
					Kind kind;
				} detail;
			};
			std::vector<Request_> request_list_;
			bool HandleAddRequest_(const Request_& request);
			bool HandleRemoveRequest_(const Request_& request);
			bool HandleChangeCollisionGroupRequest_(const Request_& request);
			bool HandleChangeCollisionWithGroundFlagRequest_(const Request_& request);
			void ProcessRequest_();

			std::vector<std::pair<CollisionGroupListType::iterator, CollisionGroupListType::iterator>> collide_group_pair_list_; //�Փ˂���O���[�v�y�A�̃��X�g

			void ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2); //�Q�̃O���[�v�Ԃł̏Փ˔���
			void ProcessCollisionInAGroup(CollisionGroupType& group); //�O���[�v���ł̏Փ˔���
		};
	}
}
