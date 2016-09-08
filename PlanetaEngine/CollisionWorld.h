#pragma once

#include <memory>
#include <unordered_map>
#include <list>
#include <functional>
#include "SceneModule.h"

namespace planeta {
	class CCollider2D;
	class CollisionGroupMatrix;
	namespace private_ {
		struct Collider2DData;
	}
	class EACollisionWithCollider2D;
	class EACollisionWithGround2D;
	namespace private_ {
		class CollisionWorld final: public private_::SceneModule
		{
		public:
			CollisionWorld();
			~CollisionWorld();
			void SetCollisionGroupMatrix(const std::shared_ptr<const CollisionGroupMatrix>& col_matrix);
			bool Resist(const private_::Collider2DData collider_data); //引数は内部でコピーされるので一時オブジェクトでよい。
			bool Remove(const CCollider2D* col_com_ptr);
			bool ChangeCollisionGroup(const CCollider2D* col_com_ptr,const std::string& group_name);
			bool ChangeCollisionWithGroundFlag(const CCollider2D* col_com_ptr, bool flag);

			void ExcuteCollisionDetection();
		private:
			void Update()override final;
			void RemoveAll();
			struct CCollider2DResistData_;
			using CollisionGroupType = std::list<std::reference_wrapper<CCollider2DResistData_>>;
			using CollisionWithGroundListType = std::list<std::reference_wrapper<CCollider2DResistData_>>;
			std::unordered_map<CCollider2D*, std::unique_ptr<CCollider2DResistData_>> collider_resist_data_map_; //コライダーの登録情報リスト
			using CollisionGroupListType = std::unordered_map<std::string, CollisionGroupType>;
			CollisionGroupListType collision_groupes_;
			CollisionWithGroundListType collision_with_ground_list_; //地形と衝突するコライダーのリスト

			std::vector<std::pair<CollisionGroupListType::iterator, CollisionGroupListType::iterator>> collide_group_pair_list_; //衝突するグループペアのリスト

			using CollisionEventQue = std::vector<std::function<void()>>;
			void ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionEventQue& collision_event_holder)const; //２つのグループ間での衝突判定
			void ProcessCollisionInAGroup(CollisionGroupType& group, CollisionEventQue& collision_event_holder)const; //グループ内での衝突判定
			void ProcessCollisionWithGround(CollisionEventQue& collision_event_holder)const; //地形との衝突判定
		};
	}
}
