#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "SceneModule.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta_engine{
	class SceneAccessorForGameObject;
	class GameObjectBase;
	class IGameObject;
	namespace core {
		class ScenePublicInterface;
		struct SceneData;
		struct SceneDataForGameObject;
	}
	namespace game{
		class GameObjectSetUpper;
		class GameObjectManager final: public core::SceneModule
			,public GameObjectManagerPublicInterface,private utility::NonCopyable<GameObjectManager>{
		public:
			GameObjectManager();
			~GameObjectManager();
			//ユーザアクセス可能関数
			utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id)override;
			utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id,const std::string& name)override;

			//設定用関数
			void RegisterGameObjectCreator(const std::string& game_object_create_id, std::function<std::shared_ptr<GameObjectBase>()>&& creator)override;

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
			/*速度適用*/
			void ApplyVelocityToGameObject();
			/*ゲームオブジェクト登録(初期化も行い、IDを返す)*/
			int RegisterGameObject(const std::shared_ptr<GameObjectBase>& go);
			int RegisterGameObject(const std::shared_ptr<GameObjectBase>& go,const std::string& name);
			/*ゲームオブジェクト登録解除(終了処理を行う)*/
			bool RemoveGameObject(int id);
			/*有効化*/
			bool ActivateGameObject(int id);
			/*無効化*/
			bool InActivateGameObject(int id);
			void TakeOver(const GameObjectManager& gom); //ゲームオブジェクトの引継ぎ処理(未実装)
			/*すべてのゲームオブジェクトを破棄*/
			void RemoveAllGameObjects();

			/*シーンデータ参照*/
			core::SceneDataForGameObject& RefSceneData() { return *scene_data_; }
		private:
			std::unordered_map<int, std::shared_ptr<GameObjectBase>> active_game_objects_;
			std::unordered_map<int, std::shared_ptr<GameObjectBase>> inactive_game_objects_;
			std::unordered_map<std::string, int> name_id_map_;
			std::vector<std::shared_ptr<GameObjectBase>> garbage_;
			std::unordered_map<std::string, std::function<bool(GameObjectBase&)>> setupper_map_; //ゲームオブジェクトセットアッパーマップ
			bool SetUpGameObject_(GameObjectBase& game_object,const std::string& setup_id)const;
			void RemoveProc_();
			int _id_counter;
			std::shared_ptr<SceneAccessorForGameObject> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameObject> scene_data_;
		};
	}
}