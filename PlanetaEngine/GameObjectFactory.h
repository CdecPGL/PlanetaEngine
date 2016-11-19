#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "WeakPointer.h"

namespace planeta {
	namespace private_ {
		class ISceneInternal;
		class GameObjectBase;
		class GameObjectFactory {
		public:
			/*新しいゲームオブジェクトを取得する*/
			std::shared_ptr<GameObjectBase> GetNewGameObject(const std::string& game_object_def_file_id, const WeakPointer<private_::ISceneInternal>& scene_data);
			std::shared_ptr<GameObjectBase> GetNewGameObject(const std::vector<std::string>& game_object_component_type_id_list, const WeakPointer<private_::ISceneInternal>& scene_data);
		private:
			std::unordered_map<std::string, std::shared_ptr<GameObjectBase>> game_object_templates_;
			/*コンポーネントリストからゲームオブジェクトを作成*/
			std::shared_ptr<GameObjectBase> CreateGameObjectFromComponentTypeList_(const std::vector<std::string>& game_object_component_type_id_list, const WeakPointer<private_::ISceneInternal>& scene_data);
			/*定義ファイルからゲームオブジェクトを作成*/
			std::shared_ptr<GameObjectBase> CreateGameObjectFromFile_(const std::string& game_object_def_file_id, const WeakPointer<private_::ISceneInternal>& scene_data);
			/*ゲームオブジェクトのクローンをゲームオブジェクトテンプレートから作成*/
			std::shared_ptr<GameObjectBase> CloneGameObjectFromTemplate_(const std::shared_ptr<GameObjectBase>& go_temp, const WeakPointer<private_::ISceneInternal>& scene_data);
		};
	}
}
