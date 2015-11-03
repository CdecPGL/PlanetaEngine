#pragma once

#include <unordered_map>
#include <memory>
#include "Object.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace system_processes {
		class GameObjectUpdateProcess;
	}
	namespace core {
		class ScenePublicInterface;
		class SceneAccessorForGameObject;
	}
	namespace game{
		class GameObject;
		class GameObjectSetUpper;
		class GameObjectManager final: public core::Object,public GameObjectManagerPublicInterface{
		public:
			GameObjectManager();
			~GameObjectManager()=default;
			//ユーザアクセス可能関数
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper,const std::string& name)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name)override;

			//システム関数
			/*シーンセット*/
			void SetScene(core::ScenePublicInterface& spi);
			/*初期化*/
			bool Initialize();
			/*終了処理*/
			bool Finalize();
			/*管理処理*/
			bool Process();
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

		private:
			std::unordered_map<int, std::shared_ptr<GameObject>> active_game_objects_;
			std::unordered_map<int, std::shared_ptr<GameObject>> inactive_game_objects_;
			std::unordered_map<std::string, int> name_id_map_;
			int _id_counter;
			std::shared_ptr<core::SceneAccessorForGameObject> scene_accessor_;
			utility::WeakPointer<system_processes::GameObjectUpdateProcess> game_object_update_process_;
			bool RemoveFromUpdateProcess_(int id);
		};
	}
}