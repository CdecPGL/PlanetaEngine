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
	class GameObjectBase;
	class IGameObject;
	namespace private_ {
		struct SceneData;
		class GameObjectTemplateHolder;
	}
	class GameObjectManager final : public private_::SceneModule
		, public GameObjectManagerPublicInterface, private util::NonCopyable<GameObjectManager> {
	public:
		GameObjectManager();
		~GameObjectManager();
		//ユーザアクセス可能関数
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_type_id, const std::string& file_id)override;
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::string& name)override;
		util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& game_object_type_id)override;
		util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& game_object_type_id, const std::string& name)override;

		//システム関数
		/*初期化*/
		bool Initialize()override;
		/*終了処理*/
		void Finalize()override;
		/*シーンデータセット*/
		void SetSceneData(const util::WeakPointer<private_::SceneData>& scene_data)override;

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
		util::WeakPointer<private_::SceneData> scene_data_;
		
		/*ゲームオブジェクトテンプレートホルダー*/
		std::unique_ptr<private_::GameObjectTemplateHolder> game_object_template_holder_;

		/*ゲームオブジェクトを作成し、セットアップを行う(作成、シーンデータのセットを行う)*/
		std::shared_ptr<GameObjectBase> CreateAndSetUpGameObject_(const std::string& game_object_type_id, const std::string& resource_id);

		/*ゲームオブジェクト登録(登録、マネージャコネクション設定、初期化を行い、IDを返す)*/
		int RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go);
		int RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go, const std::string& name);
	};
}