#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "planeta/WeakPointer.hpp"

namespace plnt {
	namespace private_ {
		class ISceneInternal;
		class GameObjectBase;
		/// <summary>ゲームオブジェクトの生成クラス。</summary>
		class GameObjectFactory {
		public:
			/// <summary>ゲームオブジェクトリソースIDを指定して新しいゲームオブジェクトを取得する。</summary>
			/// <param name="game_object_resource_id">取得するゲームオブジェクトのリソースID</param>
			/// <param name="scene_data">対象のシーンデータ</param>
			/// <returns>新しいゲームオブジェクト</returns>
			std::shared_ptr<GameObjectBase> GetNewGameObject(const std::string& game_object_resource_id, const WeakPointer<private_::ISceneInternal>& scene_data);
			/// <summary>コンポーネントリストから新しいゲームオブジェクトを取得する。</summary>
			/// <param name="game_object_component_type_id_list">ゲームオブジェクトコンポーネントタイプIDのリスト</param>
			/// <param name="scene_data">対象のシーンデータ</param>
			/// <returns>新しいゲームオブジェクト</returns>
			std::shared_ptr<GameObjectBase> GetNewGameObject(const std::vector<std::string>& game_object_component_type_id_list, const WeakPointer<private_::ISceneInternal>& scene_data);
		private:
			/// <summary>The game object templates.</summary>
			std::unordered_map<std::string, std::shared_ptr<GameObjectBase>> game_object_templates_;
			/// <summary>コンポーネントリストからゲームオブジェクトを作成する。</summary>
			/// <param name="game_object_component_type_id_list">ゲームオブジェクトコンポーネントタイプIDのリスト</param>
			/// <param name="scene_data">作成先のシーンデータ</param>
			/// <returns>作成されたゲームオブジェクト</returns>
			std::shared_ptr<GameObjectBase> CreateGameObjectFromComponentTypeList_(const std::vector<std::string>& game_object_component_type_id_list, const WeakPointer<private_::ISceneInternal>& scene_data);
			/// <summary>ゲームオブジェクトリソースIDを指定してゲームオブジェクトを作成する。</summary>
			/// <param name="game_object_resource_id">作成するゲームオブジェクトのリソースID</param>
			/// <param name="scene_data">作成先のシーンデータ</param>
			/// <returns>作成されたゲームオブジェクト</returns>
			std::shared_ptr<GameObjectBase> CreateGameObjectFromResource_(const std::string& game_object_resource_id, const WeakPointer<private_::ISceneInternal>& scene_data);
			/// <summary>ゲームオブジェクトのクローンをゲームオブジェクトテンプレートから作成する。</summary>
			/// <param name="go_temp">作成元のテンプレート</param>
			/// <param name="scene_data">作成先のシーンデータ</param>
			/// <returns>作成されたゲームオブジェクト</returns>
			std::shared_ptr<GameObjectBase> CloneGameObjectFromTemplate_(const std::shared_ptr<GameObjectBase>& go_temp, const WeakPointer<private_::ISceneInternal>& scene_data);
		};
	}
}
