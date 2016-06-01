#pragma once

#include "IGameObject.h"
#include "NonOwingPointer.h"

namespace planeta {
	//ここで定義された関数は、GameObjectBaseを継承したクラスと、GameObjectComponentに公開される。
	class IGameObjectForComponent : public IGameObject{
	public:
		//コンポーネントを型で取得する。(GameObject内部ではComponentに非所有ポインタを用いてアクセスするため、IGameObjectの同名関数を隠蔽)
		template<class ComT>
		util::NonOwingPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo_(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		//コンポーネントを型で全て取得する。
		template<class ComT>
		std::vector<util::NonOwingPointer<ComT>> GetAllComponents()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			auto lst = std::move(GetAllComponentsByTypeInfo(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
			std::vector<util::NonOwingPointer<ComT>> out();
			for (auto&& com : lst) {
				out.push_back(std::static_pointer_cast<ComT>(com));
			}
			return std::move(lst);
		}
		//ゲームオブジェクトを作成
		virtual util::WeakPointer<IGameObject> CreateGameObject(const std::string& id) = 0;
		//ゲームオブジェクトを作成して有効化
		virtual util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id) = 0;
	private:
		virtual std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const = 0;
	};
}