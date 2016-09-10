﻿#include "CollisionWorld.h"
#include "boost/next_prior.hpp"
#include "IGameObject.h"
#include "CollisionDetectFunctions.h"
#include "CCollider2D.h"
#include "CGround2D.h"
#include "EACollisionWithCollider2D.h"
#include "EACollisionWithGround2D.h"
#include "CollisionGroupMatrix.h"
#include "SystemLog.h"
#include "CTransform2D.h"
#include "Collider2DData.h"

namespace planeta{
	namespace private_ {
		struct CollisionWorld::CCollider2DResistData_ {
			private_::Collider2DData collider2d_data; //コライダーデータ
			std::unordered_map<std::string, CollisionGroupType>::iterator group_iterator_at_collision_groups; //コリジョングループのグループリスト内でのイテレータ
			CollisionGroupType::iterator iterator_at_collision_group; //コリジョングループ内でのイテレータ
			bool collidable_with_ground_flag; //地形との衝突可能フラグ
			CollisionWithGroundListType::iterator iterator_at_collision_with_ground_list; //地形と衝突する場合の、地形衝突リスト内でのイテレータ
			bool is_collided_with_ground_last_proc; //前回の判定で地面と衝突していたかどうか
		};

		CollisionWorld::CollisionWorld() = default;
		CollisionWorld::~CollisionWorld() = default;

		void CollisionWorld::Update()
		{
			
		}

		void CollisionWorld::ProcessCollisionInAGroup(CollisionGroupType& group, CollisionEventQue& collision_event_que)const {
			for (auto ccc_it = group.begin(); ccc_it != group.end(); ++ccc_it) {
				for (auto ccc_it2 = boost::next(ccc_it); ccc_it2 != group.end(); ++ccc_it2) {
					if ((*ccc_it).get().collider2d_data.collider2d.DetectCollision((*ccc_it2).get().collider2d_data.collider2d)) {
						//衝突していたら衝突イベントをホルダーに追加
						EACollisionWithCollider2D cea0((*ccc_it2).get().collider2d_data.game_object);
						collision_event_que.push_back([eve = (*ccc_it).get().collider2d_data.collide_with_collider_event_evoker, arg = cea0]() {eve(arg); });
						EACollisionWithCollider2D cea1((*ccc_it).get().collider2d_data.game_object);
						collision_event_que.push_back([eve = (*ccc_it2).get().collider2d_data.collide_with_collider_event_evoker, arg = cea1]() {eve(arg); });
					}
				}
			}
		}

		void CollisionWorld::ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionEventQue& collision_event_que)const {
			for (auto ccc_it = group1.begin(); ccc_it != group1.end(); ++ccc_it) {
				for (auto ccc_it2 = group2.begin(); ccc_it2 != group2.end(); ++ccc_it2) {
					if ((*ccc_it).get().collider2d_data.collider2d.DetectCollision((*ccc_it2).get().collider2d_data.collider2d)) {
						//衝突していたら衝突イベントをホルダーに追加
						EACollisionWithCollider2D cea0((*ccc_it2).get().collider2d_data.game_object);
						collision_event_que.push_back([eve = (*ccc_it).get().collider2d_data.collide_with_collider_event_evoker, arg = cea0]() {eve(arg); });
						EACollisionWithCollider2D cea1((*ccc_it).get().collider2d_data.game_object);
						collision_event_que.push_back([eve = (*ccc_it2).get().collider2d_data.collide_with_collider_event_evoker, arg = cea1]() {eve(arg); });
					}
				}
			}
		}

		void CollisionWorld::ProcessCollisionWithGround(CollisionEventQue& collision_event_que)const {
			for (const auto& col_com : collision_with_ground_list_) {
				auto& col_reg_data = col_com.get();
				if (col_reg_data.collider2d_data.collider2d.DetectCollision(col_reg_data.collider2d_data.transform2d.ground())) {
					//地形衝突イベントをホルダーに追加
					EACollisionWithGround2D cwgea;
					cwgea.collision_state = col_reg_data.is_collided_with_ground_last_proc ? CollisionState::Stay : CollisionState::Enter;
					collision_event_que.push_back([eve = col_reg_data.collider2d_data.collide_with_ground_event_evoker, arg = cwgea]() {eve(arg); });
					col_reg_data.is_collided_with_ground_last_proc = true; //地面との衝突状況を更新
				} else {
					if (col_reg_data.is_collided_with_ground_last_proc) { //前回衝突していたら
						//地面衝突イベントをホルダーに追加
						EACollisionWithGround2D cwgea;
						cwgea.collision_state = CollisionState::Exit;
						collision_event_que.push_back([eve = col_reg_data.collider2d_data.collide_with_ground_event_evoker, arg = cwgea]() {eve(arg); });
						col_reg_data.is_collided_with_ground_last_proc = false; //地面との衝突状況を更新
					}
				}
			}
		}

		bool CollisionWorld::Resist(const private_::Collider2DData collider_data) {
			std::unique_ptr<CCollider2DResistData_> ccrd = std::unique_ptr<CCollider2DResistData_>(new CCollider2DResistData_{ collider_data,});
			ccrd->is_collided_with_ground_last_proc = false; //地面と衝突していないとして初期化する
			const Collider2DData& col_dat = ccrd->collider2d_data;
			CCollider2D& ccol = col_dat.collider2d;
			std::string group_name = ccol.collision_group();
			//衝突グループ確認
			auto it = collision_groupes_.find(ccol.collision_group());
			if (it == collision_groupes_.end()) {
				PE_LOG_WARNING("存在しない衝突グループ", ccol.collision_group(), "が指定されたためコライダーを登録できませんでした。");
				return false; 
			}
			//衝突グループに登録
			ccrd->group_iterator_at_collision_groups = it;
			it->second.push_front(*ccrd);
			ccrd->iterator_at_collision_group = it->second.begin();

			//地形衝突コライダーリストに登録
			ccrd->collidable_with_ground_flag = ccol.is_collidable_with_ground();
			if (ccrd->collidable_with_ground_flag) {
				collision_with_ground_list_.push_front(*ccrd);
				ccrd->iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
			}
			//登録情報を登録
			collider_resist_data_map_.emplace(&ccol, std::move(ccrd));
			return true;
		}

		bool CollisionWorld::Remove(const CCollider2D* col_com_ptr) {
			auto it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (it == collider_resist_data_map_.end()) {//登録されていないコライダー
				PE_LOG_FATAL("存在しないコライダーが指定されました。");
				return false;
			} 
			if (it->second->is_collided_with_ground_last_proc) { //最後に地面と衝突した状態だったら、地面との衝突終了イベントを送っておく
				EACollisionWithGround2D e;
				e.collision_state = CollisionState::Exit;
				it->second->collider2d_data.collide_with_ground_event_evoker(e);
			}
			//地形衝突コライダーリストから除去
			if (it->second->collidable_with_ground_flag) {
				collision_with_ground_list_.erase(it->second->iterator_at_collision_with_ground_list);
			}
			//衝突グループから除去
			it->second->group_iterator_at_collision_groups->second.erase(it->second->iterator_at_collision_group);
			//登録情報から除去
			collider_resist_data_map_.erase(it);
			return true;
		}

		bool CollisionWorld::ChangeCollisionGroup(const CCollider2D* col_com_ptr, const std::string& group_name) {
			auto resist_data_it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (resist_data_it == collider_resist_data_map_.end()) { //登録されていないコライダー
				PE_LOG_FATAL("存在しないコライダーが指定されました。");
				return false;
			}
			if (resist_data_it->second->group_iterator_at_collision_groups->first == group_name) { return true; } //変更の必要はない
			auto group_it = collision_groupes_.find(group_name);
			if (group_it == collision_groupes_.end()) { //指定されたグループが存在しない
				PE_LOG_WARNING("存在しない衝突グループ", group_name, "が指定されました。");
				return false;
			}
			//現在の衝突グループから除去
			resist_data_it->second->group_iterator_at_collision_groups->second.erase(resist_data_it->second->iterator_at_collision_group);
			//新しい衝突グループに登録
			resist_data_it->second->group_iterator_at_collision_groups = group_it;
			group_it->second.push_front(*(resist_data_it->second));
			resist_data_it->second->iterator_at_collision_group = group_it->second.begin();
			return true;
		}

		bool CollisionWorld::ChangeCollisionWithGroundFlag(const CCollider2D* col_com_ptr, bool flag) {
			auto resist_data_it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (resist_data_it == collider_resist_data_map_.end()) { //登録されていないコライダー
				PE_LOG_FATAL("存在しないコライダーが指定されました。");
				return false;
			}

			if (resist_data_it->second->collidable_with_ground_flag == flag) { return true; } //変更は必要ない
			resist_data_it->second->collidable_with_ground_flag = flag;
			if (flag) { //地形衝突コライダーリストに登録
				collision_with_ground_list_.push_front(*(resist_data_it->second));
				resist_data_it->second->iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
			} else { //地形衝突コライダーリストから除去
				collision_with_ground_list_.erase(resist_data_it->second->iterator_at_collision_with_ground_list);
			}
			return true;
		}

		void CollisionWorld::SetCollisionGroupMatrix(const std::shared_ptr<const CollisionGroupMatrix>& col_matrix) { 
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

		void CollisionWorld::RemoveAll() {
			for (auto& group : collision_groupes_) {
				group.second.clear();
			}
			collide_group_pair_list_.clear();
			collider_resist_data_map_.clear();
		}

		void CollisionWorld::ExcuteCollisionDetection() {
			CollisionEventQue col_eve_que; //衝突イベントQue
			//コライダー同士
			for (const auto& gp : collide_group_pair_list_) {
				if (gp.first == gp.second) { ProcessCollisionInAGroup(gp.first->second, col_eve_que); } //同一グループの場合
				else { ProcessCollisionBetweenTwoGroups(gp.first->second, gp.second->second, col_eve_que); } //違うグループの場合
			}
			//コライダーとその所属地形
			ProcessCollisionWithGround(col_eve_que);
			//衝突イベントの伝達
			for (auto& eve : col_eve_que) {
				eve();
			}
		}

	}
}

