#include "game.hpp"
#include "config_manager.hpp"
#include "standard_collision_world.hpp"

#include <ranges>

#include "boost/next_prior.hpp"
#include "i_game_object.hpp"
#include "planeta/buildin/CCollider2D.hpp"
#include "planeta/buildin/CGround2D.hpp"
#include "e_collision_with_collider_2d.hpp"
#include "e_collision_with_ground_2d.hpp"
#include "collision_group_matrix.hpp"
#include "log_utility.hpp"
#include "planeta/buildin/CTransform2D.hpp"
#include "collider_2d_data.hpp"
#include "i_debug_manager.hpp"
#include "collider_component_2d_debug_drawer.hpp"

namespace plnt::private_ {
	struct standard_collision_world::c_collider_2d_register_data {
		collider_2d_data collider2d_data; //コライダーデータ
		std::unordered_map<std::string, collision_group_type>::iterator group_iterator_at_collision_groups;
		//コリジョングループのグループリスト内でのイテレータ
		collision_group_type::iterator iterator_at_collision_group; //コリジョングループ内でのイテレータ
		bool collidable_with_ground_flag; //地形との衝突可能フラグ
		collision_with_ground_list_type::iterator iterator_at_collision_with_ground_list; //地形と衝突する場合の、地形衝突リスト内でのイテレータ
		bool is_collided_with_ground_last_proc; //前回の判定で地面と衝突していたかどうか
	};

	standard_collision_world::standard_collision_world() = default;

	standard_collision_world::~standard_collision_world() = default;

	void standard_collision_world::update() {}

	std::pair<int, int> standard_collision_world::process_collision_in_a_group(
		collision_group_type &group, collision_event_que &collision_event_que) {
		int collision_process_count{0}, collision_count{0};
		for (auto ccc_it = group.begin(); ccc_it != group.end(); ++ccc_it) {
			for (auto ccc_it2 = boost::next(ccc_it); ccc_it2 != group.end(); ++ccc_it2) {
				++collision_process_count;
				if (ccc_it->get().collider2d_data.collider2d.detect_collision(
					ccc_it2->get().collider2d_data.collider2d)) {
					++collision_count;
					//衝突していたら衝突イベントをホルダーに追加
					e_collision_with_collider_2d cea0(ccc_it2->get().collider2d_data.collider2d);
					collision_event_que.emplace_back(
						[eve = ccc_it->get().collider2d_data.collide_with_collider_event_invoker, arg = cea0] {
							eve(arg);
						});
					e_collision_with_collider_2d cea1(ccc_it->get().collider2d_data.collider2d);
					collision_event_que.emplace_back(
						[eve = ccc_it2->get().collider2d_data.collide_with_collider_event_invoker, arg = cea1] {
							eve(arg);
						});
				}
			}
		}
		return {collision_process_count, collision_count};
	}

	std::pair<int, int> standard_collision_world::process_collision_between_two_groups(
		collision_group_type &group1, collision_group_type &group2, collision_event_que &collision_event_que) {
		int collision_process_count{0}, collision_count{0};
		for (auto ccc_it = group1.begin(); ccc_it != group1.end(); ++ccc_it) {
			for (auto ccc_it2 = group2.begin(); ccc_it2 != group2.end(); ++ccc_it2) {
				++collision_process_count;
				if (ccc_it->get().collider2d_data.collider2d.detect_collision(
					ccc_it2->get().collider2d_data.collider2d)) {
					++collision_count;
					//衝突していたら衝突イベントをホルダーに追加
					e_collision_with_collider_2d cea0(ccc_it2->get().collider2d_data.collider2d);
					collision_event_que.emplace_back(
						[eve = ccc_it->get().collider2d_data.collide_with_collider_event_invoker, arg = cea0] {
							eve(arg);
						});
					e_collision_with_collider_2d cea1(ccc_it->get().collider2d_data.collider2d);
					collision_event_que.emplace_back(
						[eve = ccc_it2->get().collider2d_data.collide_with_collider_event_invoker, arg = cea1] {
							eve(arg);
						});
				}
			}
		}
		return {collision_process_count, collision_count};
	}

	std::pair<int, int> standard_collision_world::process_collision_with_ground(
		collision_event_que &collision_event_que) const {
		int collision_process_count{0}, collision_count{0};
		for (const auto &col_com : collision_with_ground_list_) {
			++collision_process_count;
			if (auto &[collider2d_data, group_iterator_at_collision_groups, iterator_at_collision_group,
					collidable_with_ground_flag, iterator_at_collision_with_ground_list,
					is_collided_with_ground_last_proc]
				= col_com.get(); collider2d_data.collider2d.detect_collision(
				collider2d_data.transform2d.ground())) {
				++collision_count;
				//地形衝突イベントをホルダーに追加
				e_collision_with_ground_2d col_eve_args;
				col_eve_args.collision_state = is_collided_with_ground_last_proc
					                               ? collision_state::stay
					                               : collision_state::enter;
				collision_event_que.emplace_back(
					[eve = collider2d_data.collide_with_ground_event_invoker, arg = col_eve_args] { eve(arg); });
				is_collided_with_ground_last_proc = true; //地面との衝突状況を更新
			} else {
				if (is_collided_with_ground_last_proc) {
					//前回衝突していたら
					//地面衝突イベントをホルダーに追加
					e_collision_with_ground_2d col_eve_args;
					col_eve_args.collision_state = collision_state::exit;
					collision_event_que.emplace_back(
						[eve = collider2d_data.collide_with_ground_event_invoker, arg = col_eve_args] { eve(arg); });
					is_collided_with_ground_last_proc = false; //地面との衝突状況を更新
				}
			}
		}
		return {collision_process_count, collision_count};
	}

	bool standard_collision_world::resist(const collider_2d_data &collider_data) {
		auto register_data = std::make_unique<c_collider_2d_register_data>(
			c_collider_2d_register_data{collider_data,});
		register_data->is_collided_with_ground_last_proc = false; //地面と衝突していないとして初期化する
		const auto &[collider2d, game_object, transform2d, collide_with_collider_event_invoker,
			collide_with_ground_event_invoker] = register_data->collider2d_data;
		c_collider_2d &col_2d = collider2d;
		std::string group_name = col_2d.collision_group();
		//衝突グループ確認
		const auto it = collision_groups_.find(col_2d.collision_group());
		if (it == collision_groups_.end()) {
			PE_LOG_WARNING("存在しない衝突グループ", col_2d.collision_group(), "が指定されたためコライダーを登録できませんでした。");
			return false;
		}
		//衝突グループに登録
		register_data->group_iterator_at_collision_groups = it;
		it->second.emplace_front(*register_data);
		register_data->iterator_at_collision_group = it->second.begin();

		//地形衝突コライダーリストに登録
		register_data->collidable_with_ground_flag = col_2d.is_collidable_with_ground();
		if (register_data->collidable_with_ground_flag) {
			collision_with_ground_list_.emplace_front(*register_data);
			register_data->iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
		}
		//登録情報を登録
		collider_resist_data_map_.emplace(&col_2d, std::move(register_data));
		return true;
	}

	bool standard_collision_world::remove(const c_collider_2d *col_com_ptr) {
		const auto it = collider_resist_data_map_.find(const_cast<c_collider_2d *>(col_com_ptr));
		if (it == collider_resist_data_map_.end()) {
			//登録されていないコライダー
			PE_LOG_FATAL("存在しないコライダーが指定されました。");
			return false;
		}
		if (it->second->is_collided_with_ground_last_proc) {
			//最後に地面と衝突した状態だったら、地面との衝突終了イベントを送っておく
			e_collision_with_ground_2d e;
			e.collision_state = collision_state::exit;
			it->second->collider2d_data.collide_with_ground_event_invoker(e);
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

	bool standard_collision_world::change_collision_group(const c_collider_2d *col_com_ptr,
	                                                      const std::string &group_name) {
		const auto resist_data_it = collider_resist_data_map_.find(const_cast<c_collider_2d *>(col_com_ptr));
		if (resist_data_it == collider_resist_data_map_.end()) {
			//登録されていないコライダー
			PE_LOG_FATAL("存在しないコライダーが指定されました。");
			return false;
		}
		if (resist_data_it->second->group_iterator_at_collision_groups->first == group_name) { return true; }
		//変更の必要はない
		const auto group_it = collision_groups_.find(group_name);
		if (group_it == collision_groups_.end()) {
			//指定されたグループが存在しない
			PE_LOG_WARNING("存在しない衝突グループ", group_name, "が指定されました。");
			return false;
		}
		//現在の衝突グループから除去
		resist_data_it->second->group_iterator_at_collision_groups->second.erase(
			resist_data_it->second->iterator_at_collision_group);
		//新しい衝突グループに登録
		resist_data_it->second->group_iterator_at_collision_groups = group_it;
		group_it->second.emplace_front(*(resist_data_it->second));
		resist_data_it->second->iterator_at_collision_group = group_it->second.begin();
		return true;
	}

	bool standard_collision_world::change_collision_with_ground_flag(const c_collider_2d *col_com_ptr, bool flag) {
		const auto resist_data_it = collider_resist_data_map_.find(const_cast<c_collider_2d *>(col_com_ptr));
		if (resist_data_it == collider_resist_data_map_.end()) {
			//登録されていないコライダー
			PE_LOG_FATAL("存在しないコライダーが指定されました。");
			return false;
		}

		if (resist_data_it->second->collidable_with_ground_flag == flag) { return true; } //変更は必要ない
		resist_data_it->second->collidable_with_ground_flag = flag;
		if (flag) {
			//地形衝突コライダーリストに登録
			collision_with_ground_list_.emplace_front(*(resist_data_it->second));
			resist_data_it->second->iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
		} else {
			//地形衝突コライダーリストから除去
			collision_with_ground_list_.erase(resist_data_it->second->iterator_at_collision_with_ground_list);
		}
		return true;
	}

	void standard_collision_world::set_collision_group_matrix() {
		for (const auto collision_group_list = game::instance().config_manager()->collision_group_matrix().
		                                                        get_collision_group_list(); const auto &group_name :
		     collision_group_list) { collision_groups_.emplace(group_name, collision_group_type()); }

		const auto collisionable_group_pair_list = game::instance().
		                                           config_manager()->collision_group_matrix().
		                                           get_collisionable_group_pair_list();
		collide_group_pair_list_.reserve(collisionable_group_pair_list.size());
		for (const auto &[group1, group2] : collisionable_group_pair_list) {
			auto it1 = collision_groups_.find(group1);
			assert(it1 != collision_groups_.end());
			auto it2 = collision_groups_.find(group2);
			assert(it2 != collision_groups_.end());
			collide_group_pair_list_.emplace_back(it1, it2);
		}
	}

	void standard_collision_world::remove_all() {
		for (auto &groups : collision_groups_ | std::views::values) { groups.clear(); }
		collide_group_pair_list_.clear();
		collider_resist_data_map_.clear();
	}

	void standard_collision_world::execute_collision_detection() {
		//衝突カウンタをリセット
		collision_process_count_ = 0;
		collision_count_ = 0;
		//衝突イベントQue
		collision_event_que col_eve_que;
		//コライダー同士
		for (const auto &[group1, group2] : collide_group_pair_list_) {
			//同一グループの場合
			if (group1 == group2) {
				const auto [proc_cnt, col_cnt] = process_collision_in_a_group(group1->second, col_eve_que);
				collision_process_count_ += proc_cnt;
				collision_count_ += col_cnt;
			}
			//違うグループの場合
			else {
				const auto [proc_cnt, col_cnt] = process_collision_between_two_groups(
					group1->second, group2->second, col_eve_que);
				collision_process_count_ += proc_cnt;
				collision_count_ += col_cnt;
			}
		}
		//コライダーとその所属地形
		const auto [proc_cnt, col_cnt] = process_collision_with_ground(col_eve_que);
		collision_process_count_ += proc_cnt;
		collision_count_ += col_cnt;
		//衝突イベントの伝達
		for (auto &eve : col_eve_que) { eve(); }
	}

	bool standard_collision_world::initialize() {
		set_collision_group_matrix();
		//デバッグ描画を作成
		game::instance().debug_manager()->create_debug_draw_channel("CollisionSystem",
		                                                            [this]<typename T>(T &&ph1) {
			                                                            debug_draw_handler(std::forward<T>(ph1));
		                                                            });
		return true;
	}

	void standard_collision_world::debug_information_add_handle(i_debug_information_adder &di_adder) {
		di_adder.add_line("-----CollisionSystem-----");
		di_adder.add_line_v("コライダー数:", collider_resist_data_map_.size());
		di_adder.add_line_v("地面と衝突可能なコライダー数:", collision_with_ground_list_.size());
		di_adder.add_line_v("衝突判定回数:", collision_process_count_);
		di_adder.add_line_v("衝突回数:", collision_count_);
	}

	void standard_collision_world::debug_draw_handler(i_debug_drawer &dd) const {
		collider_component_2d_debug_drawer drawer{dd};
		for (const auto &register_data : collider_resist_data_map_ | std::views::values) {
			//衝突判定に使うダブルディスパッチを用いてコライダーごとに描画処理を分ける。
			register_data->collider2d_data.collider2d.detect_collision(drawer);
		}
	}

	void standard_collision_world::finalize() {
		//デバッグ描画を破棄
		game::instance().debug_manager()->delete_debug_draw_channel("CollisionSystem");
	}
}
