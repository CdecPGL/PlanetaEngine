#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <list>
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta {
	class GameObjectComponent;
	class GameObjectComponentHolder : private util::NonCopyable<GameObjectComponentHolder>{
	public:
		//コンポーネントを登録する
		void RegisterComponent(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& t_info);
		//タイプでコンポーネントを取得
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const;
		//コンポーネントを型で取得する。
		template<class ComT>
		std::shared_ptr<ComT> GetComponent() {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must derive GameComponent.");
			auto ptr = GetComponentByTypeInfo(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; });
			return ptr ? std::static_pointer_cast<ComT>(ptr) : nullptr;
		}
		//タイプに一致するコンポーネントをすべて取得する。
		const std::vector<std::shared_ptr<GameObjectComponent>>& GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const;
		//コンポーネントを型で全て取得する。
		template<class ComT>
		std::vector<std::shared_ptr<ComT>> GetAllComponents() {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must derive GameComponent.");
			const auto& go_list = GetAllComponentsByTypeInfo(typeid(ComT), [](GameObjectComponent* com) {return dynamic_cast<C*>(com) != nullptr; });
			std::vector<std::shared_ptr<ComT>> ret_list;
			for (const auto& go : go_list) {
				assert(dynamic_pointer_cast<ComT>(go) != nullptr);
				ret_list.push_back(static_pointer_cast<ComT>(go));
			}
			return std::move(ret_list);
		}
		//コンポーネントに操作を適用
		template<typename Ret>
		bool DoAllWithCheck(Ret(GameObjectComponent::* pfunc)(),const Ret& true_value) {
			for (auto&& com : component_list_) {
				if (((*com).*pfunc)() != true_value) { return false; }
			}
			return true;
		}
		template<typename Ret>
		void DoAll(Ret(GameObjectComponent::* pfunc)()) {
			for (auto&& com : component_list_) {
				((*com).*pfunc)();
			}
		}
	private:
		//コンポーネントリスト
		std::list<std::shared_ptr<GameObjectComponent>> component_list_;
		//タイプによるコンポーネントマップ<typeindex,<完全探索済みか(false:少なくとも１つは探索済み,true:全て探索済み),コンポーネントリスト>>
		mutable std::unordered_map<std::type_index, std::pair<bool, std::vector<std::shared_ptr<GameObjectComponent>>>> component_type_map_;
		//タイプマップにコンポーネントを追加
		void AddComponentToTypeInfoMap(const std::type_info& ti, const std::shared_ptr<GameObjectComponent>& com);
	};
}
