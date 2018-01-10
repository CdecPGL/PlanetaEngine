#pragma once

#include <memory>
#include <unordered_map>
#include <list>
#include <functional>
#include "planeta/CollisionWorld.hpp"

namespace plnt {
	class EACollisionWithCollider2D;
	class EACollisionWithGround2D;
	class IDebugDrawer;
	namespace private_ {
		class StandardCollisionWorld final: public CollisionWorld
		{
		public:
			StandardCollisionWorld();
			~StandardCollisionWorld();
			bool Initialize()override;
			void Finalize()override;
			bool Resist(const private_::Collider2DData& collider_data)override; //引数は内部でコピーされるので一時オブジェクトでよい。
			bool Remove(const CCollider2D* col_com_ptr)override;
			bool ChangeCollisionGroup(const CCollider2D* col_com_ptr,const std::string& group_name)override;
			bool ChangeCollisionWithGroundFlag(const CCollider2D* col_com_ptr, bool flag)override;
			void ExcuteCollisionDetection()override;
		private:
			void SetCollisionGroupMatrix();
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

			//衝突判定回数
			int collision_process_count_ = 0;
			//衝突発生回数
			int collision_count_ = 0;

			using CollisionEventQue = std::vector<std::function<void()>>;
			//衝突判定実行関数。戻り値は<判定回数,衝突回数>
			std::pair<int,int> ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionEventQue& collision_event_holder)const; //２つのグループ間での衝突判定
			std::pair<int, int> ProcessCollisionInAGroup(CollisionGroupType& group, CollisionEventQue& collision_event_holder)const; //グループ内での衝突判定
			std::pair<int, int> ProcessCollisionWithGround(CollisionEventQue& collision_event_holder)const; //地形との衝突判定

			void DebugInformationAddHandle(IDebugInformationAdder& di_adder) override;
			void DebugDrawHandler(IDebugDrawer& dd);

		};
	}
}
