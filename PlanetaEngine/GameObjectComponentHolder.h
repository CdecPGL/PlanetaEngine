#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include "WeakPointer.h"

namespace planeta_engine {
	class GameObjectComponent;
	class GameObjectComponentHolder {
	public:
		//コンポーネントを追加する。
		template<class ComT>
		utility::WeakPointer<ComT> CreateAndAddComponent() {
			static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "ComT must derive GameComponent.");

		}
		//コンポーネントを型で取得する。
		template<class ComT>
		utility::WeakPointer<ComT> GetComponent() {
			static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "ComT must derive GameComponent.");
			auto ptr = GetComponentByTypeInfo(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT>(goc) != nullptr; });
			return ptr ? std::static_pointer_cast<ComT>(ptr) : nullptr;
		}
		//コンポーネントを型で全て取得する。
		template<class ComT>
		std::vector<utility::WeakPointer<ComT>> GetAllComponents() {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must derive GameComponent.");
			const auto& go_list = GetAllComponentsByTypeInfo(typeid(ComT), [](GameObjectComponent* com) {return dynamic_cast<C*>(com) != nullptr; });
			std::vector<utility::WeakPointer<ComT>> ret_list;
			for (const auto& go : go_list) {
				assert(dynamic_pointer_cast<ComT>(go) != nullptr);
				ret_list.push_back(static_pointer_cast<ComT>(go));
			}
			return std::move(ret_list);
		}
	private:
		//コンポーネントリスト
		std::unordered_map<int, std::shared_ptr<GameObjectComponent>> component_list_;
		//タイプによるコンポーネントマップ<typeindex,<完全探索済みか(false:少なくとも１つは探索済み,true:全て探索済み),コンポーネントリスト>>
		mutable std::unordered_map<std::type_index, std::pair<bool, std::vector<std::shared_ptr<GameObjectComponent>>>> component_type_map_;
		//タイプマップにコンポーネントを追加
		void AddComponentToTypeInfoMap(const std::type_info& ti, const std::shared_ptr<GameObjectComponent>& com);
		//タイプでコンポーネントを取得
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const;
		//タイプに一致するコンポーネントをすべて取得する。
		const std::vector<std::shared_ptr<GameObjectComponent>>& GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const;
	};
}
