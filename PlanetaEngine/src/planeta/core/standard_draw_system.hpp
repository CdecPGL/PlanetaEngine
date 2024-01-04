#pragma once

#include <unordered_map>
#include <map>
#include <list>
#include <memory>

#include "draw_system.hpp"

namespace plnt {
	class screen_drawer_2d;
	class screen_drawer_gui;

	namespace private_ {
		class screen;

		class standard_draw_system final : public draw_system {
		public:
			standard_draw_system();
			standard_draw_system(const standard_draw_system &) = delete;
			standard_draw_system(standard_draw_system &&) = delete;
			~standard_draw_system() override;
			standard_draw_system &operator=(const standard_draw_system &) = delete;
			standard_draw_system &operator=(standard_draw_system &&) = delete;

			bool initialize() override;
			void finalize() override;
			void update() override;
			void execute_draw() override;
			void execute_draw_gui() override;
			void apply_camera_state() override;
			/*描画コンポーネント登録*/
			std::unique_ptr<c_draw_2d_manager_connection> register_c_draw_2d(const std::shared_ptr<c_draw_2d> &draw_component,
			                                                             int priority) override;
			std::unique_ptr<c_draw_gui_manager_connection> register_c_draw_gui(
				const std::shared_ptr<c_draw_gui> &draw_component,
				int priority) override;

			/*カメラコンポーネント登録*/
			std::unique_ptr<c_camera_2d_manager_connection> register_c_camera_2d(
				const std::shared_ptr<c_camera_2d> &camera_component) override;

			/*デバッグ用*/
			void debug_information_add_handle(i_debug_information_adder &di_adder) override;

			/*! GUI座標をGameObject座空間標に変換*/
			[[nodiscard]] vector_2dd covert_position_screen_space_to_game_object_space(
				const vector_2di &gui_space_pos) const override;
			/*! GameObject座標をGUI座標に変換*/
			[[nodiscard]] vector_2di covert_position_game_object_space_to_screen_space(
				const vector_2dd &game_object_space_pos) const override;

			//コンポーネントリスト型
			template <class ComType>
			using com_list_type = std::list<ComType *>;
			//コンポーネント更新リスト型
			template <class ComType>
			using update_list_type = std::map<int, com_list_type<ComType>>;

			//コンポーネント登録情報
			template <class ComType>
			struct register_data {
				int priority;
				std::shared_ptr<ComType> com_ptr;
				typename update_list_type<ComType>::iterator update_list_iterator;
				typename com_list_type<ComType>::iterator priority_com_list_iterator;
				bool is_active = false;
			};

			//コンポーネントマップ型
			template <class ComType>
			using com_map_type = std::unordered_map<ComType *, register_data<ComType>>;

			//コンポーネントホルダー
			template <class ComType>
			class com_holder {
			public:
				//登録
				std::pair<bool, typename com_map_type<ComType>::iterator> add(
					const std::shared_ptr<ComType> &com, int priority) {
					register_data<ComType> rd;
					rd.priority = priority;
					rd.com_ptr = com;
					auto ret = com_map_.emplace(com.get(), rd);
					if (!ret.second) { return {false, {}}; }
					++all_count_;
					return {true, ret.first};
				}

				//有効化
				bool activate(typename com_map_type<ComType>::iterator com_map_itr) {
					register_data<ComType> &rd = com_map_itr->second;
					if (rd.is_active) { return false; }
					//アップデートリストに登録
					update_list_[rd.priority].push_back(rd.com_ptr.get());
					rd.update_list_iterator = update_list_.find(rd.priority);
					rd.priority_com_list_iterator = --rd.update_list_iterator->second.end();
					++active_count_;

					rd.is_active = true;
					return true;
				}

				//無効化
				bool inactivate(typename com_map_type<ComType>::iterator com_map_itr) {
					register_data<ComType> &rd = com_map_itr->second;
					if (!rd.is_active) { return false; }
					//アップデートリストから削除
					rd.update_list_iterator->second.erase(rd.priority_com_list_iterator);
					--active_count_;

					rd.is_active = false;
					return true;
				}

				//除去
				bool remove(typename com_map_type<ComType>::iterator com_map_itr) {
					if (const register_data<ComType> &rd = com_map_itr->second; rd.is_active) {
						if (!inactivate(com_map_itr)) { return false; }
					}
					//コンポーネントマップから削除
					com_map_.erase(com_map_itr);
					--all_count_;
					return true;
				}

				//優先度変更
				bool change_priority(typename com_map_type<ComType>::iterator com_map_itr, int priority) {
					if (register_data<ComType> &rd = com_map_itr->second; rd.is_active) {
						if (!inactivate(com_map_itr)) { return false; }
						rd.priority = priority;
						if (!activate(com_map_itr)) { return false; }
					} else { rd.priority = priority; }
					return true;
				}

				//走査
				void iterate(const std::function<void(ComType &)> &proc) {
					for (auto &dcl : update_list_) { for (auto &com : dcl.second) { proc(*com); } }
				}

				//情報取得
				[[nodiscard]] int all_count() const { return all_count_; }
				[[nodiscard]] int active_count() const { return active_count_; }

			private:
				//更新リスト
				update_list_type<ComType> update_list_;
				//Comマップ
				com_map_type<ComType> com_map_;
				//管理情報
				int all_count_ = 0;
				int active_count_ = 0;
			};

		private:
			//CDraw2Dホルダー
			com_holder<c_draw_2d> c_draw_2d_holder_;
			//CDrawGUIホルダー
			com_holder<c_draw_gui> c_draw_gui_holder_;

			/*描画画面*/
			std::shared_ptr<screen> screen_;
			std::unique_ptr<screen_drawer_2d> screen_drawer_2d_;
			std::unique_ptr<screen_drawer_gui> screen_drawer_gui_;
			/*カメラコンポーネント*/
			std::shared_ptr<c_camera_2d> camera2d_;
		};
	}
}
