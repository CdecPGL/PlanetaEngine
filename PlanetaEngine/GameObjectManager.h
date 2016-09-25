#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "SceneModule.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta {
	class SceneAccessorForGameObject;
	class IGameObject;
	namespace private_ {
		struct SceneData;
		class GameObjectFactory;
		class GameObjectBase;
		class GameObjectManager final : public private_::SceneModule
			, public GameObjectManagerPublicInterface, private util::NonCopyable<GameObjectManager> {
		public:
			GameObjectManager();
			~GameObjectManager();
			//ユーザアクセス可能関数
			WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_def_file_id)override;
			WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_def_file_id, const std::string& name)override;
			WeakPointer<IGameObject> CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list)override;
			WeakPointer<IGameObject> CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list, const std::string& name)override;

			//システム関数
			/*初期化*/
			bool Initialize()override;
			/*終了処理*/
			void Finalize()override;
			/*シーンデータセット*/
			void SetSceneData(const WeakPointer<private_::SceneData>& scene_data)override;

			/*管理処理*/
			void Update()override;
			/*ゲームオブジェクト登録解除(終了処理を行う)*/
			bool RemoveGameObject(int id);
			/*有効化*/
			bool ActivateGameObject(int id);
			/*無効化*/
			bool InActivateGameObject(int id);
			/*すべてのゲームオブジェクトを破棄*/
			void RemoveAllGameObjects();
		private:
			std::unordered_map<int, std::shared_ptr<GameObjectBase>> active_game_objects_;
			std::unordered_map<int, std::shared_ptr<GameObjectBase>> inactive_game_objects_;
			std::unordered_map<std::string, int> name_id_map_;
			std::vector<std::shared_ptr<GameObjectBase>> garbage_;
			void RemoveProc_();
			int _id_counter;
			WeakPointer<private_::SceneData> scene_data_;

			/*ゲームオブジェクトテンプレートホルダー*/
			std::unique_ptr<private_::GameObjectFactory> game_object_factory_;

			/*ゲームオブジェクトを作成し、セットアップを行う(作成、シーンデータのセットを行う)*/
			std::shared_ptr<GameObjectBase> CreateAndSetUpGameObject_(const std::string& game_object_file_id);
			std::shared_ptr<GameObjectBase> CreateAndSetUpGameObject_(const std::vector<std::string>& com_type_list);

			/*ゲームオブジェクト登録(登録、マネージャコネクション設定、初期化を行い、IDを返す)*/
			int RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go);
			int RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go, const std::string& name);
		};
	}
}