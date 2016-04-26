#include "TCollisionDetect.h"
#include "boost/next_prior.hpp"
#include "IGameObjectAccessor.h"
#include "CollisionDetectFunctions.h"
#include "CCollider2D.h"
#include "CGround2D.h"
#include "CollisionEventArgument.h"
#include "CollisionWithGroundEventArgument.h"
#include "CollisionGroupMatrix.h"
#include "SystemLog.h"
#include "CTransform2D.h"

namespace planeta_engine{
	namespace system_processes {
		TCollisionDetect::~TCollisionDetect()
		{
		}

		void TCollisionDetect::Update()
		{
			//コライダー同士
			CollisionColliderEventHolderType collision_collider_event_holder;
			for (const auto& gp : collide_group_pair_list_) {
				if (gp.first->second == gp.second->second) { ProcessCollisionInAGroup(gp.first->second, collision_collider_event_holder); } //同一グループの場合
				else { ProcessCollisionBetweenTwoGroups(gp.first->second, gp.second->second, collision_collider_event_holder); } //違うグループの場合
			}
			//コライダーとその所属地形
			CollisionGroundEventHolderType collision_ground_event_holder;
			ProcessCollisionWithGround(collision_ground_event_holder);
			//コライダー衝突イベントの伝達
			for (auto& eve : collision_collider_event_holder) {
				eve.first->collided(eve.second);
			}
			//地形衝突イベントの伝達
			for (auto& eve : collision_ground_event_holder) {
				eve.first->collided_with_ground(eve.second);
			}
		}

		void TCollisionDetect::ProcessCollisionInAGroup(CollisionGroupType& group, CollisionColliderEventHolderType& collision_event_holder)const {
			for (auto ccc_it = group.begin(); ccc_it != group.end(); ++ccc_it) {
				for (auto ccc_it2 = boost::next(ccc_it); ccc_it2 != group.end(); ++ccc_it2) {
					if ((*ccc_it)->DetectCollision(**ccc_it2)) {
						//衝突していたら衝突イベントをホルダーに追加
						event_arguments::CollisionEventArgument cea0((*ccc_it2)->game_object());
						collision_event_holder.push_back(std::make_pair(*ccc_it, cea0));
						event_arguments::CollisionEventArgument cea1((*ccc_it)->game_object());
						collision_event_holder.push_back(std::make_pair(*ccc_it2, cea1));
					}
				}
			}
		}

		void TCollisionDetect::ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionColliderEventHolderType& collision_event_holder)const {
			for (auto ccc_it = group1.begin(); ccc_it != group1.end(); ++ccc_it) {
				for (auto ccc_it2 = group2.begin(); ccc_it2 != group2.end(); ++ccc_it2) {
					if ((*ccc_it)->DetectCollision(**ccc_it2)) {
						//衝突していたら衝突イベントをホルダーに追加
						event_arguments::CollisionEventArgument cea0((*ccc_it2)->game_object());
						collision_event_holder.push_back(std::make_pair(*ccc_it, cea0));
						event_arguments::CollisionEventArgument cea1((*ccc_it)->game_object());
						collision_event_holder.push_back(std::make_pair(*ccc_it2, cea1));
					}
				}
			}
		}

		void TCollisionDetect::ProcessCollisionWithGround(CollisionGroundEventHolderType& collision_event_holder)const {
			for (const auto& col_com : collision_with_ground_list_) {
				if (col_com->DetectCollision(col_com->game_object().transform().ground())) {
					//衝突していたら地形衝突イベントをホルダーに追加
					event_arguments::CollisionWithGroundEventArgument cwgea;
					collision_event_holder.push_back(std::make_pair(col_com, cwgea));
				}
			}
		}

		bool TCollisionDetect::Resist(const std::shared_ptr<CCollider2D>& col_com) {
			ColliderComponentResistData_ ccrd;
			ccrd.pointer = col_com;
			std::string group_name = col_com->collision_group();
			//衝突グループ確認
			auto it = collision_groupes_.find(col_com->collision_group());
			if (it == collision_groupes_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "存在しない衝突グループ", col_com->collision_group(), "が指定されたためコライダーを登録できませんでした。");
				return false; 
			}
			//衝突グループに登録
			ccrd.group_iterator_at_collision_groups = it;

			it->second.push_front(col_com.get());
			ccrd.iterator_at_collision_group = it->second.begin();
			//地形衝突コライダーリストに登録
			ccrd.collision_with_ground_flag = col_com->collide_with_ground_flag();
			if (ccrd.collision_with_ground_flag) {
				collision_with_ground_list_.push_front(col_com.get());
				ccrd.iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
			}
			//登録情報を登録
			collider_resist_data_map_.emplace(col_com.get(), ccrd);
			return true;
		}

		bool TCollisionDetect::Remove(const CCollider2D* col_com_ptr) {
			auto it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (it == collider_resist_data_map_.end()) {//登録されていないコライダー
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないコライダーが指定されました。");
				assert(false);
				return false;
			} 
			//地形衝突コライダーリストから除去
			if (it->second.collision_with_ground_flag) {
				collision_with_ground_list_.erase(it->second.iterator_at_collision_with_ground_list);
			}
			//衝突グループから除去
			it->second.group_iterator_at_collision_groups->second.erase(it->second.iterator_at_collision_group);
			return true;
		}

		bool TCollisionDetect::ChangeCollisionGroup(const CCollider2D* col_com_ptr, const std::string& group_name) {
			auto resist_data_it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (resist_data_it == collider_resist_data_map_.end()) { //登録されていないコライダー
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないコライダーが指定されました。");
				assert(false);
				return false;
			}
			if (resist_data_it->second.group_iterator_at_collision_groups->first == group_name) { return true; } //変更の必要はない
			auto group_it = collision_groupes_.find(group_name);
			if (group_it == collision_groupes_.end()) { //指定されたグループが存在しない
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "存在しない衝突グループ", group_name, "が指定されました。");
				return false;
			}
			//現在の衝突グループから除去
			resist_data_it->second.group_iterator_at_collision_groups->second.erase(resist_data_it->second.iterator_at_collision_group);
			//新しい衝突グループに登録
			resist_data_it->second.group_iterator_at_collision_groups = group_it;
			group_it->second.push_front(resist_data_it->second.pointer.get());
			resist_data_it->second.iterator_at_collision_group = group_it->second.begin();
			return true;
		}

		bool TCollisionDetect::ChangeCollisionWithGroundFlag(const CCollider2D* col_com_ptr, bool flag) {
			auto resist_data_it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (resist_data_it == collider_resist_data_map_.end()) { //登録されていないコライダー
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないコライダーが指定されました。");
				assert(false);
				return false;
			}

			if (resist_data_it->second.collision_with_ground_flag == flag) { return true; } //変更は必要ない
			resist_data_it->second.collision_with_ground_flag = flag;
			if (flag) { //地形衝突コライダーリストに登録
				collision_with_ground_list_.push_front(resist_data_it->second.pointer.get());
				resist_data_it->second.iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
			} else { //地形衝突コライダーリストから除去
				collision_with_ground_list_.erase(resist_data_it->second.iterator_at_collision_with_ground_list);
			}
			return true;
		}

		void TCollisionDetect::SetCollisionGroupMatrix(const std::shared_ptr<const core::CollisionGroupMatrix>& col_matrix) { 
			auto collision_group_list = std::move(col_matrix->GetCollisionGroupList());
			for (const auto& group_name : collision_group_list) {
				collision_groupes_.emplace(group_name, CollisionGroupType());
			}

			auto collisionable_group_pair_list = std::move(col_matrix->GetCollisionableGroupPairList());
			collide_group_pair_list_.reserve(collisionable_group_pair_list.size());
			for (const auto& collisionable_group_pair : collisionable_group_pair_list) {
				auto it1 = collision_groupes_.find(collisionable_group_pair.first);
				assert(it1 != collision_groupes_.end());
				auto it2 = collision_groupes_.find(collisionable_group_pair.second);
				assert(it2 != collision_groupes_.end());
				collide_group_pair_list_.push_back(std::make_pair(it1, it2));
			}
		}

		void TCollisionDetect::RemoveAll() {
			for (auto& group : collision_groupes_) {
				group.second.clear();
			}
			collide_group_pair_list_.clear();
			collider_resist_data_map_.clear();
		}
	}
}

