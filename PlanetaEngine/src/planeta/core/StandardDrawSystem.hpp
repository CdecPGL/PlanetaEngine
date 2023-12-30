﻿#pragma once

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

		class StandardDrawSystem final : public draw_system {
		public:
			StandardDrawSystem();
			~StandardDrawSystem();
			bool initialize() override;
			void finalize() override;
			void update() override;
			void execute_draw() override;
			void execute_draw_gui() override;
			void apply_camera_state() override;
			/*描画コンポーネント登録*/
			std::unique_ptr<CDraw2DManagerConnection> register_c_draw_2d(const std::shared_ptr<CDraw2D> &draw_component,
			                                                          int priority) override;
			std::unique_ptr<CDrawGUIManagerConnection> register_c_draw_gui(const std::shared_ptr<CDrawGUI> &draw_component,
			                                                            int priority) override;

			/*カメラコンポーネント登録*/
			std::unique_ptr<CCamera2DManagerConnection> register_c_camera_2d(
				const std::shared_ptr<CCamera2D> &camera_component) override;

			/*デバッグ用*/
			void debug_information_add_handle(i_debug_information_adder &di_adder) override;

			/*! GUI座標をGameObject座空間標に変換*/
			Vector2Dd covert_position_screen_space_to_game_object_space(const Vector2Di &gui_space_pos) const override;
			/*! GameObject座標をGUI座標に変換*/
			Vector2Di covert_position_game_object_space_to_screen_space(const Vector2Dd &gameobject_space_pos) const override;

			//コンポーネントリスト型
			template <class ComType>
			using ComListType = std::list<ComType *>;
			//コンポーネント更新リスト型
			template <class ComType>
			using UpdateListType = std::map<int, ComListType<ComType>>;

			//コンポーネント登録情報
			template <class ComType>
			struct RegisterData {
				int priority;
				std::shared_ptr<ComType> com_ptr;
				typename UpdateListType<ComType>::iterator update_list_iterator;
				typename ComListType<ComType>::iterator prioriry_com_list_iterator;
				bool is_active = false;
			};

			//コンポーネントマップ型
			template <class ComType>
			using ComMapType = std::unordered_map<ComType *, RegisterData<ComType>>;

			//コンポーネントホルダー
			template <class ComType>
			class ComHolder {
			public:
				//登録
				std::pair<bool, typename ComMapType<ComType>::iterator> Register(
					const std::shared_ptr<ComType> &com, int priority) {
					RegisterData<ComType> rd;
					rd.priority = priority;
					rd.com_ptr = com;
					auto ret = com_map_.emplace(com.get(), rd);
					if (!ret.second) { return {false, {}}; }
					++all_count_;
					return {true, ret.first};
				}

				//有効化
				bool Activate(typename ComMapType<ComType>::iterator com_map_itr) {
					RegisterData<ComType> &rd = com_map_itr->second;
					if (rd.is_active) { return false; }
					//アップデートリストに登録
					update_list_[rd.priority].push_back(rd.com_ptr.get());
					rd.update_list_iterator = update_list_.find(rd.priority);
					rd.prioriry_com_list_iterator = --rd.update_list_iterator->second.end();
					++active_count_;

					rd.is_active = true;
					return true;
				}

				//無効化
				bool Inactivate(typename ComMapType<ComType>::iterator com_map_itr) {
					RegisterData<ComType> &rd = com_map_itr->second;
					if (!rd.is_active) { return false; }
					//アップデートリストから削除
					rd.update_list_iterator->second.erase(rd.prioriry_com_list_iterator);
					--active_count_;

					rd.is_active = false;
					return true;
				}

				//除去
				bool Remove(typename ComMapType<ComType>::iterator com_map_itr) {
					const RegisterData<ComType> &rd = com_map_itr->second;
					if (rd.is_active) { if (!Inactivate(com_map_itr)) { return false; } }
					//コンポーネントマップから削除
					com_map_.erase(com_map_itr);
					--all_count_;
					return true;
				}

				//優先度変更
				bool ChangePriority(typename ComMapType<ComType>::iterator com_map_itr, int priority) {
					RegisterData<ComType> &rd = com_map_itr->second;
					if (rd.is_active) {
						if (!Inactivate(com_map_itr)) { return false; }
						rd.priority = priority;
						if (!Activate(com_map_itr)) { return false; }
					} else { rd.priority = priority; }
					return true;
				}

				//走査
				void Iterate(const std::function<void(ComType &)> &proc) {
					for (auto &dcl : update_list_) { for (auto &com : dcl.second) { proc(*com); } }
				}

				//情報取得
				int all_count() const { return all_count_; }
				int active_count() const { return active_count_; }

			private:
				//更新リスト
				UpdateListType<ComType> update_list_;
				//Comマップ
				ComMapType<ComType> com_map_;
				//管理情報
				int all_count_ = 0;
				int active_count_ = 0;
			};

		private:
			//CDraw2Dホルダー
			ComHolder<CDraw2D> cdraw2d_holder_;
			//CDrawGUIホルダー
			ComHolder<CDrawGUI> cdrawgui_holder_;

			/*描画画面*/
			std::shared_ptr<screen> screen_;
			std::unique_ptr<screen_drawer_2d> screen_drawer_2d_;
			std::unique_ptr<screen_drawer_gui> screen_drawer_gui_;
			/*カメラコンポーネント*/
			std::shared_ptr<CCamera2D> camera2d_;
		};
	}
}
