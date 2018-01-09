#pragma once

#include <string>
#include <functional>
#include <vector>
#include "WeakPointer.h"

namespace planeta {
	class IGameObject;
	class IGameObjectManager {
	public:
		virtual ~IGameObjectManager() = default;
		/*定義ファイルを指定してゲームオブジェクトを作成*/
		virtual WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_def_file_id) = 0;
		/*定義ファイルを指定して名前付きゲームオブジェクトを作成*/
		virtual WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_def_file_id, const std::string& name) = 0;
		/*コンポーネントタイプIDを指定してゲームオブジェクトを作成*/
		virtual WeakPointer<IGameObject> CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list) = 0;
		/*コンポーネントタイプIDを指定して名前付きゲームオブジェクトを作成*/
		virtual WeakPointer<IGameObject> CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list, const std::string& name) = 0;
	};
}