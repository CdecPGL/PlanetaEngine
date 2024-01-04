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
		class standard_collision_world final : public collision_world {
		public:
			standard_collision_world();
			standard_collision_world(const standard_collision_world &) = delete;
			standard_collision_world(standard_collision_world &&) = delete;
			~standard_collision_world() override;
			standard_collision_world &operator=(const standard_collision_world &) = delete;
			standard_collision_world &operator=(standard_collision_world &&) = delete;

			bool initialize() override;
			void finalize() override;
			bool resist(const collider_2d_data &collider_data) override; //引数は内部でコピーされるので一時オブジェクトでよい。
			bool remove(const c_collider_2d *col_com_ptr) override;
			bool change_collision_group(const c_collider_2d *col_com_ptr, const std::string &group_name) override;
			bool change_collision_with_ground_flag(const c_collider_2d *col_com_ptr, bool flag) override;
			void execute_collision_detection() override;

		private:
			void set_collision_group_matrix();
			void update() override;
			void remove_all();
			struct c_collider_2d_register_data;
			using collision_group_type = std::list<std::reference_wrapper<c_collider_2d_register_data>>;
			using collision_with_ground_list_type = std::list<std::reference_wrapper<c_collider_2d_register_data>>;
			std::unordered_map<c_collider_2d *, std::unique_ptr<c_collider_2d_register_data>> collider_resist_data_map_;
			//コライダーの登録情報リスト
			using collision_group_list_type = std::unordered_map<std::string, collision_group_type>;
			collision_group_list_type collision_groups_;
			collision_with_ground_list_type collision_with_ground_list_; //地形と衝突するコライダーのリスト

			std::vector<std::pair<collision_group_list_type::iterator, collision_group_list_type::iterator>>
			collide_group_pair_list_; //衝突するグループペアのリスト

			//衝突判定回数
			int collision_process_count_ = 0;
			//衝突発生回数
			int collision_count_ = 0;

			using collision_event_que = std::vector<std::function<void()>>;
			//衝突判定実行関数。戻り値は<判定回数,衝突回数>
			static std::pair<int, int> process_collision_between_two_groups(collision_group_type &group1,
			                                                                collision_group_type &group2,
			                                                                collision_event_que &collision_event_que);
			//２つのグループ間での衝突判定
			static std::pair<int, int> process_collision_in_a_group(collision_group_type &group,
			                                                        collision_event_que &collision_event_que);
			//グループ内での衝突判定
			std::pair<int, int> process_collision_with_ground(collision_event_que &collision_event_que) const;
			//地形との衝突判定

			void debug_information_add_handle(i_debug_information_adder &di_adder) override;
			void debug_draw_handler(i_debug_drawer &dd) const;
		};
	}
}
