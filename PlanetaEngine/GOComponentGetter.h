#pragma once

#include <vector>
#include <functional>
#include "NonOwingPointer.h"

namespace planeta {
	class GameObjectComponentHolder;
	class GameObjectComponent;
	/*! @brief ゲームオブジェクトコンポーネントを取得するためのクラス
		
		GameObjectComponetが自信を所有するゲームオブジェクトのコンポーネントを取得するために使われることを想定し、util::NonOwingPointerで返す。<br/>
		GameObjectComponent::GetComponentsProcの引数として渡される。
	*/
	class GOComponentGetter {
	public:
		GOComponentGetter(const GameObjectComponentHolder& com_holder);
		//! コンポーネントを型で取得する。
		template<class ComT>
		util::NonOwingPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo_(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		//! コンポーネントを型で全て取得する。
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
	private:
		const GameObjectComponentHolder& com_holder_;
		std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const;
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const;
	};
}