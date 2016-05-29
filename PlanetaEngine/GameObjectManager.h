#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "SceneModule.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	class GameObjectBase;
	class IGameObject;
	namespace core {
		struct SceneData;
	}
	class GameObjectManager final : public core::SceneModule
		, public GameObjectManagerPublicInterface, private utility::NonCopyable<GameObjectManager> {
	public:
		GameObjectManager();
		~GameObjectManager();
		//ユーザアクセス可能関数
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id)override;
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id, const std::string& name)override;

		//システム関数
		/*初期化*/
		bool Initialize()override;
		/*終了処理*/
		void Finalize()override;
		/*シーンデータセット*/
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data)override;

		/*管理処理*/
		void Update()override;
		/*ゲームオブジェクト登録解除(終了処理を行う)*/
		bool RemoveGameObject(int id);
		/*有効化*/
		bool ActivateGameObject(int id);
		/*無効化*/
		bool InActivateGameObject(int id);
		void TakeOver(const GameObjectManager& gom); //ゲームオブジェクトの引継ぎ処理(未実装)
		/*すべてのゲームオブジェクトを破棄*/
		void RemoveAllGameObjects();
	private:
		std::unordered_map<int, std::shared_ptr<GameObjectBase>> active_game_objects_;
		std::unordered_map<int, std::shared_ptr<GameObjectBase>> inactive_game_objects_;
		std::unordered_map<std::string, int> name_id_map_;
		std::vector<std::shared_ptr<GameObjectBase>> garbage_;
		void RemoveProc_();
		int _id_counter;
		utility::WeakPointer<core::SceneData> scene_data_;
		static std::shared_ptr<GameObjectBase >CreateGameObjectByID_(const std::string& id);
		/*ゲームオブジェクト登録(初期化も行い、IDを返す)*/
		int RegisterAndSetUpGameObject_(const std::shared_ptr<GameObjectBase>& go);
		int RegisterAndSetUpGameObject_(const std::shared_ptr<GameObjectBase>& go, const std::string& name);
	};
}