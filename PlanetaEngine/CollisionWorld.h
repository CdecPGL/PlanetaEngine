#pragma once

#include <memory>
#include <unordered_map>
#include <list>
#include "SceneModule.h"

namespace planeta_engine {
	class CCollider2D;
	namespace core {
		class CollisionGroupMatrix;
	}
	class EACollisionWithCollider2D;
	class EACollisionWithGround2D;
	namespace core {
		class CollisionWorld final: public core::SceneModule
		{
		public:
			~CollisionWorld();
			void SetCollisionGroupMatrix(const std::shared_ptr<const core::CollisionGroupMatrix>& col_matrix);
			bool Resist(const std::shared_ptr<CCollider2D>& col_com);
			bool Remove(const CCollider2D* col_com_ptr);
			bool ChangeCollisionGroup(const CCollider2D* col_com_ptr,const std::string& group_name);
			bool ChangeCollisionWithGroundFlag(const CCollider2D* col_com_ptr, bool flag);

			void ExcuteCollisionDetection();
		private:
			void Update()override final;
			void RemoveAll();
			using CollisionGroupType = std::list<CCollider2D*>;
			using CollisionWithGroundListType = std::list<CCollider2D*>;
			struct CCollider2DResistData_ {
				std::shared_ptr<CCollider2D> pointer; //コライダーのポインター
				std::unordered_map<std::string, CollisionGroupType>::iterator group_iterator_at_collision_groups; //コリジョングループのグループリスト内でのイテレータ
				CollisionGroupType::iterator iterator_at_collision_group; //コリジョングループ内でのイテレータ
				bool collision_with_ground_flag; //地形との衝突フラグ
				CollisionWithGroundListType::iterator iterator_at_collision_with_ground_list; //地形と衝突する場合の、地形衝突リスト内でのイテレータ
			};
			std::unordered_map<CCollider2D*, CCollider2DResistData_> collider_resist_data_map_; //コライダーの登録情報リスト
			using CollisionGroupListType = std::unordered_map<std::string, CollisionGroupType>;
			CollisionGroupListType collision_groupes_; //衝突グループ
			CollisionWithGroundListType collision_with_ground_list_; //地形と衝突するコライダーのリスト

			std::vector<std::pair<CollisionGroupListType::iterator, CollisionGroupListType::iterator>> collide_group_pair_list_; //衝突するグループペアのリスト

			using CollisionColliderEventHolderType = std::vector<std::pair<CCollider2D*, EACollisionWithCollider2D>>;
			using CollisionGroundEventHolderType = std::vector<std::pair<CCollider2D*, EACollisionWithGround2D>>;
			void ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionColliderEventHolderType& collision_event_holder)const; //２つのグループ間での衝突判定
			void ProcessCollisionInAGroup(CollisionGroupType& group, CollisionColliderEventHolderType& collision_event_holder)const; //グループ内での衝突判定
			void ProcessCollisionWithGround(CollisionGroundEventHolderType& collision_event_holder)const;
		};
	}
}
