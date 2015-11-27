#include "CollisionDetectProcess.h"
#include "boost/next_prior.hpp"
#include "IGameObjectAccessor.h"
#include "CollisionDetectFunctions.h"
#include "ColliderComponent.h"
#include "GroundComponent.h"
#include "CollisionEventArgument.h"
#include "CollisionWithGroundEventArgument.h"
#include "CollisionGroupMatrix.h"
#include "SystemLog.h"

namespace planeta_engine{
	namespace system_processes {
		CollisionDetectProcess::~CollisionDetectProcess()
		{
		}

		void CollisionDetectProcess::Update()
		{
			ProcessRequest_();
			//コライダー同士
			for (const auto& gp : collide_group_pair_list_) {
				if (gp.first->second == gp.second->second) { ProcessCollisionInAGroup(gp.first->second); } //同一グループの場合
				else { ProcessCollisionBetweenTwoGroups(gp.first->second, gp.second->second); } //違うグループの場合
			}
			//コライダーとその所属地形
			for (const auto& col_com : collision_with_ground_list_) {
				if (col_com->DetectCollision(col_com->game_object().belonging_ground())) {
					//衝突していたら地形衝突イベントを発生させる
					event_arguments::CollisionWithGroundEventArgument cwgea;
					col_com->collided_with_ground(cwgea);
				}
			}
		}

		void CollisionDetectProcess::ProcessCollisionInAGroup(CollisionGroupType& group) {
			for (auto ccc_it = group.begin(); ccc_it != group.end(); ++ccc_it) {
				for (auto ccc_it2 = boost::next(ccc_it); ccc_it2 != group.end(); ++ccc_it2) {
					if ((*ccc_it)->DetectCollision(**ccc_it2)) {
						//衝突していたら衝突イベントを発生させる
						event_arguments::CollisionEventArgument cea0((*ccc_it2)->game_object());
						(*ccc_it)->collided(cea0);
						event_arguments::CollisionEventArgument cea1((*ccc_it)->game_object());
						(*ccc_it2)->collided(cea1);
					}
				}
			}
		}

		void CollisionDetectProcess::ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2) {
			for (auto ccc_it = group1.begin(); ccc_it != group1.end(); ++ccc_it) {
				for (auto ccc_it2 = group2.begin(); ccc_it2 != group2.end(); ++ccc_it2) {
					if ((*ccc_it)->DetectCollision(**ccc_it2)) {
						//衝突していたら衝突イベントを発生させる
						event_arguments::CollisionEventArgument cea0((*ccc_it2)->game_object());
						(*ccc_it)->collided(cea0);
						event_arguments::CollisionEventArgument cea1((*ccc_it)->game_object());
						(*ccc_it2)->collided(cea1);
					}
				}
			}
		}

		bool CollisionDetectProcess::Resist(const std::shared_ptr<components::ColliderComponent>& col_com) {
			//衝突グループ確認
			if (collision_groupes_.find(col_com->collision_group()) == collision_groupes_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "存在しない衝突グループ", col_com->collision_group(), "が指定されたためコライダーを登録できませんでした。");
				return false; 
			}
			//リクエスト作成
			Request_ request(Request_::Kind::Add);
			request.detail.add_detail.col_com = col_com;
			request_list_.push_back(request);
			return true;
		}

		bool CollisionDetectProcess::Remove(const components::ColliderComponent* col_com_ptr) {
			auto it = collider_resist_data_map_.find(const_cast<components::ColliderComponent*>(col_com_ptr));
			if (it == collider_resist_data_map_.end()) {//登録されていないコライダー
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないコライダーが指定されました。");
				assert(false);
				return false;
			} 
			//リクエスト作成
			Request_ request(Request_::Kind::Remove);
			request.resist_data_iterator = it;
			request_list_.push_back(request);
			return true;
		}

		bool CollisionDetectProcess::ChangeCollisionGroup(const components::ColliderComponent* col_com_ptr, const std::string& group_name) {
			auto resist_data_it = collider_resist_data_map_.find(const_cast<components::ColliderComponent*>(col_com_ptr));
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
			//リクエスト作成
			Request_ request(Request_::Kind::ChangeCollisionGroup);
			request.resist_data_iterator = resist_data_it;
			request.detail.change_collision_group_detail.new_collision_group_iterator = group_it;
			request_list_.push_back(request);
			return true;
		}

		bool CollisionDetectProcess::ChangeCollisionWithGroundFlag(const components::ColliderComponent* col_com_ptr, bool flag) {
			auto resist_data_it = collider_resist_data_map_.find(const_cast<components::ColliderComponent*>(col_com_ptr));
			if (resist_data_it == collider_resist_data_map_.end()) { //登録されていないコライダー
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないコライダーが指定されました。");
				assert(false);
				return false;
			}
			if (resist_data_it->second.collision_with_ground_flag == flag) { return true; } //変更は必要ない
			//リクエスト作成
			Request_ request(Request_::Kind::ChangeCollisionWithGroundFlag);
			request.resist_data_iterator = resist_data_it;
			request.detail.change_collision_with_ground_flag_detail.new_collision_with_ground_flag = flag;
			request_list_.push_back(request);
			return true;
		}

		void CollisionDetectProcess::SetCollisionGroupMatrix(const std::shared_ptr<const core::CollisionGroupMatrix>& col_matrix) { 
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

		void CollisionDetectProcess::RemoveAll() {
			for (auto& group : collision_groupes_) {
				group.second.clear();
			}
			collide_group_pair_list_.clear();
			collider_resist_data_map_.clear();
			request_list_.clear();
		}

		void CollisionDetectProcess::ProcessRequest_() {
			for (const auto& request : request_list_) {
				switch (request.detail.kind) {
				case Request_::Kind::Add:HandleAddRequest_(request); break;
				case Request_::Kind::Remove:HandleRemoveRequest_(request); break;
				case Request_::Kind::ChangeCollisionGroup:HandleChangeCollisionGroupRequest_(request);
				case Request_::Kind::ChangeCollisionWithGroundFlag:HandleChangeCollisionWithGroundFlagRequest_(request); break;
				default:assert(false); break;
				}
			}
			request_list_.clear();
		}

		bool CollisionDetectProcess::HandleAddRequest_(const Request_& request) {
			ColliderComponentResistData_ ccrd;
			auto& col_com = request.detail.add_detail.col_com;
			ccrd.pointer = col_com;
			std::string group_name = col_com->collision_group();
			//地形衝突コライダーリストに登録
			ccrd.collision_with_ground_flag = col_com->collide_with_ground_flag();
			if (ccrd.collision_with_ground_flag) {
				collision_with_ground_list_.push_front(col_com.get());
				ccrd.iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
			}
			//衝突グループに登録
			auto it = collision_groupes_.find(group_name);
			assert(it != collision_groupes_.end()); //リクエスト時に確認されているはず
			ccrd.group_iterator_at_collision_groups = it;
			it->second.push_front(col_com.get());
			ccrd.iterator_at_collision_group = it->second.begin();
			//登録情報を登録
			collider_resist_data_map_.emplace(col_com.get(), ccrd);
			return true;
		}

		bool CollisionDetectProcess::HandleRemoveRequest_(const Request_& request) {
			auto it = request.resist_data_iterator;
			//地形衝突コライダーリストから除去
			if (it->second.collision_with_ground_flag) {
				collision_with_ground_list_.erase_after(it->second.iterator_at_collision_with_ground_list);
			}
			//衝突グループから除去
			it->second.group_iterator_at_collision_groups->second.erase_after(it->second.iterator_at_collision_group);
			return true;
		}

		bool CollisionDetectProcess::HandleChangeCollisionGroupRequest_(const Request_& request) {
			auto resist_data_it = request.resist_data_iterator;
			auto collision_group_it = request.detail.change_collision_group_detail.new_collision_group_iterator;
			//現在の衝突グループから除去
			resist_data_it->second.group_iterator_at_collision_groups->second.erase_after(resist_data_it->second.iterator_at_collision_group);
			//新しい衝突グループに登録
			resist_data_it->second.group_iterator_at_collision_groups = collision_group_it;
			collision_group_it->second.push_front(resist_data_it->second.pointer.get());
			resist_data_it->second.iterator_at_collision_group = collision_group_it->second.begin();
			return true;
		}

		bool CollisionDetectProcess::HandleChangeCollisionWithGroundFlagRequest_(const Request_& request) {
			auto resist_data_it = request.resist_data_iterator;
			bool new_flag = request.detail.change_collision_with_ground_flag_detail.new_collision_with_ground_flag;
			assert(resist_data_it->second.collision_with_ground_flag != new_flag); //変更不要の確認はリクエスト時に行われているはず
			resist_data_it->second.collision_with_ground_flag = new_flag;
			if (new_flag) { //地形衝突コライダーリストに登録
				collision_with_ground_list_.push_front(resist_data_it->second.pointer.get());
				resist_data_it->second.iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
			} else { //地形衝突コライダーリストから除去
				collision_with_ground_list_.erase_after(resist_data_it->second.iterator_at_collision_with_ground_list);
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		//CollisionDetectProcess::Request_::Detail
		//////////////////////////////////////////////////////////////////////////
		CollisionDetectProcess::Request_::Detail::Detail(Kind k) {
			switch (k) {
			case Kind::Add:new(this) AddDetail(); break;
			case Kind::Remove:new(this) RemoveDetail(); break;
			case Kind::ChangeCollisionGroup:new(this) ChangeCollisionGroupDetail(); break;
			case Kind::ChangeCollisionWithGroundFlag:new(this) ChangeCollisionWithGroundFlagDetail(); break;
			default:assert(false); break;
			}
			kind = k;
		}
		CollisionDetectProcess::Request_::Detail::~Detail() {
			switch (kind) {
			case Kind::Add:add_detail.~AddDetail(); break;
			case Kind::Remove:remove_detail.~RemoveDetail(); break;
			case Kind::ChangeCollisionGroup:change_collision_group_detail.~ChangeCollisionGroupDetail(); break;
			case Kind::ChangeCollisionWithGroundFlag:change_collision_with_ground_flag_detail.~ChangeCollisionWithGroundFlagDetail(); break;
			default:assert(false); break;
			}
		}
		CollisionDetectProcess::Request_::Detail::Detail(const Detail& d) {
			switch (d.kind) {
			case Kind::Add:new(this) AddDetail(d.add_detail); break;
			case Kind::Remove:new(this) RemoveDetail(d.remove_detail); break;
			case Kind::ChangeCollisionGroup:new(this) ChangeCollisionGroupDetail(change_collision_group_detail); break;
			case Kind::ChangeCollisionWithGroundFlag:new(this) ChangeCollisionWithGroundFlagDetail(d.change_collision_with_ground_flag_detail); break;
			default:assert(false); break;
			}
		}
	}
}

