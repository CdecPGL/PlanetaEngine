#include "planeta/Game.hpp"
#include "ConfigManager.hpp"
#include "StandardCollisionWorld.hpp"
#include "boost/next_prior.hpp"
#include "IGameObject.hpp"
#include "CollisionDetectFunctions.hpp"
#include "CCollider2D.hpp"
#include "CGround2D.hpp"
#include "EACollisionWithCollider2D.hpp"
#include "EACollisionWithGround2D.hpp"
#include "CollisionGroupMatrix.hpp"
#include "LogUtility.hpp"
#include "CTransform2D.hpp"
#include "Collider2DData.hpp"
#include "IDebugManager.hpp"
#include "ColliderComponent2DDebugDrawer.hpp"

namespace plnt{
	namespace private_ {
		struct StandardCollisionWorld::CCollider2DResistData_ {
			private_::Collider2DData collider2d_data; //コライダーデータ
			std::unordered_map<std::string, CollisionGroupType>::iterator group_iterator_at_collision_groups; //コリジョングループのグループリスト内でのイテレータ
			CollisionGroupType::iterator iterator_at_collision_group; //コリジョングループ内でのイテレータ
			bool collidable_with_ground_flag; //地形との衝突可能フラグ
			CollisionWithGroundListType::iterator iterator_at_collision_with_ground_list; //地形と衝突する場合の、地形衝突リスト内でのイテレータ
			bool is_collided_with_ground_last_proc; //前回の判定で地面と衝突していたかどうか
		};

		StandardCollisionWorld::StandardCollisionWorld() {
		}
		StandardCollisionWorld::~StandardCollisionWorld() {
		}

		void StandardCollisionWorld::Update()
		{
			
		}

		std::pair<int, int> StandardCollisionWorld::ProcessCollisionInAGroup(CollisionGroupType& group, CollisionEventQue& collision_event_que)const {
			int collision_process_count{ 0 }, collision_count{ 0 };
			for (auto ccc_it = group.begin(); ccc_it != group.end(); ++ccc_it) {
				for (auto ccc_it2 = boost::next(ccc_it); ccc_it2 != group.end(); ++ccc_it2) {
					++collision_process_count;
					if ((*ccc_it).get().collider2d_data.collider2d.DetectCollision((*ccc_it2).get().collider2d_data.collider2d)) {
						++collision_count;
						//衝突していたら衝突イベントをホルダーに追加
						EACollisionWithCollider2D cea0((*ccc_it2).get().collider2d_data.collider2d);
						collision_event_que.push_back([eve = (*ccc_it).get().collider2d_data.collide_with_collider_event_evoker, arg = cea0]() {eve(arg); });
						EACollisionWithCollider2D cea1((*ccc_it).get().collider2d_data.collider2d);
						collision_event_que.push_back([eve = (*ccc_it2).get().collider2d_data.collide_with_collider_event_evoker, arg = cea1]() {eve(arg); });
					}
				}
			}
			return{ collision_process_count, collision_count };
		}

		std::pair<int, int> StandardCollisionWorld::ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionEventQue& collision_event_que)const {
			int collision_process_count{ 0 }, collision_count{ 0 };
			for (auto ccc_it = group1.begin(); ccc_it != group1.end(); ++ccc_it) {
				for (auto ccc_it2 = group2.begin(); ccc_it2 != group2.end(); ++ccc_it2) {
					++collision_process_count;
					if ((*ccc_it).get().collider2d_data.collider2d.DetectCollision((*ccc_it2).get().collider2d_data.collider2d)) {
						++collision_count;
						//衝突していたら衝突イベントをホルダーに追加
						EACollisionWithCollider2D cea0((*ccc_it2).get().collider2d_data.collider2d);
						collision_event_que.push_back([eve = (*ccc_it).get().collider2d_data.collide_with_collider_event_evoker, arg = cea0]() {eve(arg); });
						EACollisionWithCollider2D cea1((*ccc_it).get().collider2d_data.collider2d);
						collision_event_que.push_back([eve = (*ccc_it2).get().collider2d_data.collide_with_collider_event_evoker, arg = cea1]() {eve(arg); });
					}
				}
			}
			return{ collision_process_count, collision_count };
		}

		std::pair<int, int> StandardCollisionWorld::ProcessCollisionWithGround(CollisionEventQue& collision_event_que)const {
			int collision_process_count{ 0 }, collision_count{ 0 };
			for (const auto& col_com : collision_with_ground_list_) {
				++collision_process_count;
				auto& col_reg_data = col_com.get();
				if (col_reg_data.collider2d_data.collider2d.DetectCollision(col_reg_data.collider2d_data.transform2d.ground())) {
					++collision_count;
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
			return{ collision_process_count, collision_count };
		}

		bool StandardCollisionWorld::Resist(const private_::Collider2DData& collider_data) {
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

		bool StandardCollisionWorld::Remove(const CCollider2D* col_com_ptr) {
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

		bool StandardCollisionWorld::ChangeCollisionGroup(const CCollider2D* col_com_ptr, const std::string& group_name) {
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

		bool StandardCollisionWorld::ChangeCollisionWithGroundFlag(const CCollider2D* col_com_ptr, bool flag) {
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

		void StandardCollisionWorld::SetCollisionGroupMatrix() { 
			decltype(auto) collision_group_list = Game::instance().config_manager()->collision_group_matrix().GetCollisionGroupList();
			for (const auto& group_name : collision_group_list) {
				collision_groupes_.emplace(group_name, CollisionGroupType());
			}

			decltype(auto) collisionable_group_pair_list = Game::instance().config_manager()->collision_group_matrix().GetCollisionableGroupPairList();
			collide_group_pair_list_.reserve(collisionable_group_pair_list.size());
			for (const auto& collisionable_group_pair : collisionable_group_pair_list) {
				auto it1 = collision_groupes_.find(collisionable_group_pair.first);
				assert(it1 != collision_groupes_.end());
				auto it2 = collision_groupes_.find(collisionable_group_pair.second);
				assert(it2 != collision_groupes_.end());
				collide_group_pair_list_.push_back(std::make_pair(it1, it2));
			}
		}

		void StandardCollisionWorld::RemoveAll() {
			for (auto& group : collision_groupes_) {
				group.second.clear();
			}
			collide_group_pair_list_.clear();
			collider_resist_data_map_.clear();
		}

		void StandardCollisionWorld::ExcuteCollisionDetection() {
			//衝突カウンタをリセット
			collision_process_count_ = 0;
			collision_count_ = 0;
			//衝突イベントQue
			CollisionEventQue col_eve_que;
			//コライダー同士
			for (const auto& gp : collide_group_pair_list_) {
				//同一グループの場合
				if (gp.first == gp.second) {
					auto ret = ProcessCollisionInAGroup(gp.first->second, col_eve_que); 
					collision_process_count_ += ret.first;
					collision_count_ += ret.second;
				} 
				//違うグループの場合
				else {
					auto ret = ProcessCollisionBetweenTwoGroups(gp.first->second, gp.second->second, col_eve_que);
					collision_process_count_ += ret.first;
					collision_count_ += ret.second;
				}
			}
			//コライダーとその所属地形
			auto ret = ProcessCollisionWithGround(col_eve_que);
			collision_process_count_ += ret.first;
			collision_count_ += ret.second;
			//衝突イベントの伝達
			for (auto& eve : col_eve_que) {
				eve();
			}
		}

		bool StandardCollisionWorld::Initialize() {
			SetCollisionGroupMatrix();
			//デバッグ描画を作成
			Game::instance().debug_manager()->CreateDebugDrawChannel("CollisionSystem", std::bind(&StandardCollisionWorld::DebugDrawHandler, this, std::placeholders::_1));
			return true;
		}

		void StandardCollisionWorld::DebugInformationAddHandle(IDebugInformationAdder& di_adder) {
			di_adder.AddLine("-----CollisionSystem-----");
			di_adder.AddLineV("コライダー数:", collider_resist_data_map_.size());
			di_adder.AddLineV("地面と衝突可能なコライダー数:", collision_with_ground_list_.size());
			di_adder.AddLineV("衝突判定回数:", collision_process_count_);
			di_adder.AddLineV("衝突回数:", collision_count_);
		}

		void StandardCollisionWorld::DebugDrawHandler(IDebugDrawer& dd) {
			ColliderComponent2DDebugDrawer ccdd{dd};
			for (auto&& c : collider_resist_data_map_) {
				//衝突判定に使うダブルディスパッチを用いてコライダーごとに描画処理を分ける。
				c.second->collider2d_data.collider2d.DetectCollision(ccdd);
			}
		}

		void StandardCollisionWorld::Finalize() {
			//デバッグ描画を破棄
			Game::instance().debug_manager()->DeleteDebugDrawChannel("CollisionSystem");
		}

	}
}

