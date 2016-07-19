#pragma once

#include "IGameObject.h"
#include "NonOwingPointer.h"

namespace planeta {
	/*! @brief GameObjectComponentとGameObjectのための、ゲームオブジェクトへのアクセスを提供するインターフェイスクラス
	
		ここで定義された関数は、GameObjectBaseを継承したクラスと、GameObjectComponentに公開される。
	*/
	class IGameObjectForComponent : public IGameObject{
	public:
		//! ゲームオブジェクトを作成
		virtual util::WeakPointer<IGameObject> CreateGameObject(const std::string& gameobject_type_id, const std::string& resource_id) = 0;
		//!ゲームオブジェクトを作成して有効化
		virtual util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& gameobject_type_id, const std::string& resource_id) = 0;
		//! 定義ファイルを読み込まないでゲームオブジェクトを作成
		virtual util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& gameobject_type_id) = 0;
		//! 定義ファイルを読み込まないでゲームオブジェクトを作成して有効化
		virtual util::WeakPointer<IGameObject> CreateAndActivateDefaultGameObject(const std::string& gameobject_type_id) = 0;
	};
}