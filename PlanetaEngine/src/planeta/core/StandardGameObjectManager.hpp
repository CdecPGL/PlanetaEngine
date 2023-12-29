#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "game_object_manager.hpp"

namespace plnt {
	class SceneAccessorForGameObject;
	class i_game_object;

	namespace private_ {
		class game_object_factory;
		class game_object_base;

		class StandardGameObjectManager final : public game_object_manager {
		public:
			StandardGameObjectManager();
			~StandardGameObjectManager();
			//ユーザアクセス可能関数
			WeakPointer<i_game_object> create_game_object(const std::string &game_object_def_file_id) override;
			WeakPointer<i_game_object> create_game_object(const std::string &game_object_def_file_id,
			                                          const std::string &name) override;
			WeakPointer<i_game_object> create_game_object_with_component_type_id_list(
				const std::vector<std::string> &game_object_component_type_id_list) override;
			WeakPointer<i_game_object> create_game_object_with_component_type_id_list(
				const std::vector<std::string> &game_object_component_type_id_list, const std::string &name) override;

			//システム関数
			/*初期化*/
			bool Initialize() override;
			/*終了処理*/
			void Finalize() override;

			/*管理処理*/
			void Update() override;
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
			void RemoveProc_();
			int _id_counter;

			/*ゲームオブジェクトテンプレートホルダー*/
			std::unique_ptr<private_::game_object_factory> game_object_factory_;

			/*ゲームオブジェクトを作成し、セットアップを行う(作成、シーンデータのセットを行う)*/
			std::shared_ptr<game_object_base> CreateAndSetUpGameObject_(const std::string &game_object_file_id);
			std::shared_ptr<game_object_base> CreateAndSetUpGameObject_(const std::vector<std::string> &com_type_list);

			/*ゲームオブジェクト登録(登録、マネージャコネクション設定、初期化を行い、IDを返す)*/
			int RegisterAndInitializeGameObject_(const std::shared_ptr<game_object_base> &go);
			int RegisterAndInitializeGameObject_(const std::shared_ptr<game_object_base> &go, const std::string &name);

			virtual void DebugInformationAddHandle(i_debug_information_adder &di_adder) override;
		};
	}
}
