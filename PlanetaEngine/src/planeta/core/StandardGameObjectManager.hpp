#pragma once

#include <unordered_map>
#include <memory>

#include "game_object_manager.hpp"

namespace plnt {
	class i_game_object;

	namespace private_ {
		class game_object_factory;
		class game_object_base;

		class standard_game_object_manager final : public game_object_manager {
		public:
			standard_game_object_manager();
			standard_game_object_manager(const standard_game_object_manager &) = delete;
			standard_game_object_manager(standard_game_object_manager &&) = delete;
			~standard_game_object_manager() override;
			standard_game_object_manager &operator=(const standard_game_object_manager &) = delete;
			standard_game_object_manager &operator=(standard_game_object_manager &&) = delete;

			//ユーザアクセス可能関数
			[[nodiscard]] weak_pointer<i_game_object> create_game_object(const std::string &game_object_def_file_id) override;
			[[nodiscard]] weak_pointer<i_game_object> create_game_object(const std::string &game_object_def_file_id,
			                                               const std::string &name) override;
			[[nodiscard]] weak_pointer<i_game_object> create_game_object_with_component_type_id_list(
				const std::vector<std::string> &game_object_component_type_id_list) override;
			[[nodiscard]] weak_pointer<i_game_object> create_game_object_with_component_type_id_list(
				const std::vector<std::string> &game_object_component_type_id_list, const std::string &name) override;

			//システム関数
			/*初期化*/
			bool initialize() override;
			/*終了処理*/
			void finalize() override;

			/*管理処理*/
			void update() override;
			/*ゲームオブジェクト登録解除(終了処理を行う)*/
			bool remove_game_object(int id) override;
			/*有効化*/
			bool activate_game_object(int id) override;
			/*無効化*/
			bool inactivate_game_object(int id) override;
			/*すべてのゲームオブジェクトを破棄*/
			void remove_all_game_objects() override;

		private:
			std::unordered_map<int, std::shared_ptr<game_object_base>> active_game_objects_;
			std::unordered_map<int, std::shared_ptr<game_object_base>> inactive_game_objects_;
			std::unordered_map<std::string, int> name_id_map_;
			std::vector<std::shared_ptr<game_object_base>> garbage_;
			void remove_proc();
			int id_counter_;

			/*ゲームオブジェクトテンプレートホルダー*/
			std::unique_ptr<game_object_factory> game_object_factory_;

			/*ゲームオブジェクトを作成し、セットアップを行う(作成、シーンデータのセットを行う)*/
			[[nodiscard]] std::shared_ptr<game_object_base> create_and_set_up_game_object(const std::string &game_object_file_id);
			[[nodiscard]] std::shared_ptr<game_object_base> create_and_set_up_game_object(const std::vector<std::string> &com_type_list);

			/*ゲームオブジェクト登録(登録、マネージャコネクション設定、初期化を行い、IDを返す)*/
			int register_and_initialize_game_object(const std::shared_ptr<game_object_base> &go);
			int register_and_initialize_game_object(const std::shared_ptr<game_object_base> &go, const std::string &name);

			void debug_information_add_handle(i_debug_information_adder &di_adder) override;
		};
	}
}
