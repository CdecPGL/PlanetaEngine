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
				std::shared_ptr<components::ColliderComponent> pointer; //コライダーのポインター
				std::unordered_map<std::string, CollisionGroupType>::iterator group_iterator_at_collision_groups; //コリジョングループのグループリスト内でのイテレータ
				CollisionGroupType::iterator iterator_at_collision_group; //コリジョングループ内でのイテレータ
				bool collision_with_ground_flag; //地形との衝突フラグ
				CollisionWithGroundListType::iterator iterator_at_collision_with_ground_list; //地形と衝突する場合の、地形衝突リスト内でのイテレータ
			};
			std::unordered_map<components::ColliderComponent*, ColliderComponentResistData_> collider_resist_data_map_; //コライダーの登録情報リスト
			using CollisionGroupListType = std::unordered_map<std::string, CollisionGroupType>;
			CollisionGroupListType collision_groupes_; //衝突グループ
			CollisionWithGroundListType collision_with_ground_list_; //地形と衝突するコライダーのリスト

			std::vector<std::pair<CollisionGroupListType::iterator, CollisionGroupListType::iterator>> collide_group_pair_list_; //衝突するグループペアのリスト

			using CollisionColliderEventHolderType = std::vector<std::pair<components::ColliderComponent*, event_arguments::CollisionEventArgument>>;
			using CollisionGroundEventHolderType = std::vector<std::pair<components::ColliderComponent*, event_arguments::CollisionWithGroundEventArgument>>;
			void ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionColliderEventHolderType& collision_event_holder)const; //２つのグループ間での衝突判定
			void ProcessCollisionInAGroup(CollisionGroupType& group, CollisionColliderEventHolderType& collision_event_holder)const; //グループ内での衝突判定
			void ProcessCollisionWithGround(CollisionGroundEventHolderType& collision_event_holder)const;
		};
	}
}
