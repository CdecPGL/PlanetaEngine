#pragma once

#include "IGameObject.h"
#include "NonOwingPointer.h"
#include "GameObjectComponentHolder.h"

namespace planeta_engine {
	//ここで定義された関数は、GameObjectBaseを継承したクラスと、GameObjectComponentに公開される。
	class IGameObjectForComponent : public IGameObject{
	public:
		//コンポーネントを型で取得する。
		template<class ComT>
		utility::NonOwingPointer<ComT> GetComponent() {
			return RefComponentHolder().GetComponent<ComT>();
		}
		//コンポーネントを型で全て取得する。
		template<class ComT>
		std::vector<utility::NonOwingPointer<ComT>> GetAllComponents() {
			auto lst = std::move(RefComponentHolder().GetAllComponents<ComT>());
			std::vector<utility::NonOwingPointer<ComT>> out();
			for (auto&& com : lst) {
				out.push_back(com);
			}
			return std::move(lst);
		}
		//ゲームオブジェクトを作成
		virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& id) = 0;
		//ゲームオブジェクトを作成して有効化
		virtual utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id) = 0;
	private:
		virtual  GameObjectComponentHolder& RefComponentHolder() = 0;
	};
}