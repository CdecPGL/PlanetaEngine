#pragma once

#include <unordered_map>
#include <memory>
#include "SceneModule.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class ScenePublicInterface;
		struct SceneData;
		class SceneAccessorForGameObject;
		struct SceneDataForGameObject;
	}
	namespace game{
		class GameObject;
		class GameObjectSetUpper;
		class GameObjectManager final: public core::SceneModule
			,public GameObjectManagerPublicInterface{
		public:
			GameObjectManager();
			~GameObjectManager();
			//ユーザアクセス可能関数
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper,const std::string& name)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name)override;

			//システム関数
			/*初期化*/
			bool Initialize()override;
			/*終了処理*/
			void Finalize()override;
			/*シーンインターフェイスセット*/
			void SetSceneInterface(core::ScenePublicInterface& spi)override;
			/*シーンデータセット*/
			void SetSceneData(const core::SceneData& scene_data)override;

			/*管理処理*/
			bool Process();
			/*更新*/
			void Update();
			/*ゲームオブジェクト登録(初期化も行い、IDを返す)*/
			int Resist(const std::shared_ptr<GameObject>& go);
			int Resist(const std::shared_ptr<GameObject>& go,const std::string& name);
			/*ゲームオブジェクト登録解除(終了処理を行う)*/
			bool Remove(int id);
			/*有効化*/
			bool Activate(int id);
			/*無効化*/
			bool InActivate(int id);
			void TakeOver(const GameObjectManager& gom); //ゲームオブジェクトの引継ぎ処理(未実装)
			/*すべてのゲームオブジェクトを破棄*/
			void RemoveAllGameObjects();

			/*シーンデータ参照*/
			core::SceneDataForGameObject& RefSceneData() { return *scene_data_; }
		private:
			GameObjectManager(const GameObjectManager&) = delete;
			GameObjectManager(GameObjectManager&&) = delete;
			GameObjectManager& operator=(const GameObjectManager&) = delete;
			GameObjectManager& operator=(GameObjectManager&&) = delete;

			std::unordered_map<int, std::shared_ptr<GameObject>> active_game_objects_;
			std::unordered_map<int, std::shared_ptr<GameObject>> inactive_game_objects_;
			std::unordered_map<std::string, int> name_id_map_;
			std::vector<std::shared_ptr<GameObject>> garbage_;
			void RemoveProc_();
			int _id_counter;
			std::shared_ptr<core::SceneAccessorForGameObject> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameObject> scene_data_;
		};
	}
}