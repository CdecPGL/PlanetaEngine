#pragma once

#include <memory>
#include <unordered_map>
#include <list>
#include <functional>
#include "collision_world.hpp"

namespace plnt {
	class e_collision_with_collider_2d;
	class e_collision_with_ground_2d;
	class i_debug_drawer;

	namespace private_ {
		class StandardCollisionWorld final : public collision_world {
		public:
			StandardCollisionWorld();
			~StandardCollisionWorld();
			bool initialize() override;
			void finalize() override;
			bool resist(const private_::collider_2d_data &collider_data) override; //引数は内部でコピーされるので一時オブジェクトでよい。
			bool remove(const CCollider2D *col_com_ptr) override;
			bool change_collision_group(const CCollider2D *col_com_ptr, const std::string &group_name) override;
			bool change_collision_with_ground_flag(const CCollider2D *col_com_ptr, bool flag) override;
			void execute_collision_detection() override;

		private:
			void SetCollisionGroupMatrix();
			void update() override final;
			void RemoveAll();
			struct CCollider2DResistData_;
			using CollisionGroupType = std::list<std::reference_wrapper<CCollider2DResistData_>>;
			using CollisionWithGroundListType = std::list<std::reference_wrapper<CCollider2DResistData_>>;
			std::unordered_map<CCollider2D *, std::unique_ptr<CCollider2DResistData_>> collider_resist_data_map_;
			//コライダーの登録情報リスト
			using CollisionGroupListType = std::unordered_map<std::string, CollisionGroupType>;
			CollisionGroupListType collision_groupes_;
			CollisionWithGroundListType collision_with_ground_list_; //地形と衝突するコライダーのリスト

			std::vector<std::pair<CollisionGroupListType::iterator, CollisionGroupListType::iterator>>
			collide_group_pair_list_; //衝突するグループペアのリスト

			//衝突判定回数
			int collision_process_count_ = 0;
			//衝突発生回数
			int collision_count_ = 0;

			using CollisionEventQue = std::vector<std::function<void()>>;
			//衝突判定実行関数。戻り値は<判定回数,衝突回数>
			std::pair<int, int> ProcessCollisionBetweenTwoGroups(CollisionGroupType &group1, CollisionGroupType &group2,
			                                                     CollisionEventQue &collision_event_holder) const;
			//２つのグループ間での衝突判定
			std::pair<int, int> ProcessCollisionInAGroup(CollisionGroupType &group,
			                                             CollisionEventQue &collision_event_holder) const; //グループ内での衝突判定
			std::pair<int, int> ProcessCollisionWithGround(CollisionEventQue &collision_event_holder) const; //地形との衝突判定

			void debug_information_add_handle(i_debug_information_adder &di_adder) override;
			void DebugDrawHandler(i_debug_drawer &dd);
		};
	}
}
