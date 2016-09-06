#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <vector>
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta {
	class GameObjectComponent;
	/*GOコンポーネントを管理するクラス
	GOCエイリアスはゲームオブジェクトによって変わるので、ゲームオブジェクト内でしかアクセスするべきでない。
	*/
	class GameObjectComponentHolder : private util::NonCopyable<GameObjectComponentHolder>{
	public:
		//コンポーネントを登録する
		bool RegisterComponent(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& t_info,const std::string& alias);
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
		void CloneToOtherHolder(GameObjectComponentHolder& com_holder);
		//エイリアスインデックスマップの参照
		const auto& alias_idx_map()const { return alias_idx_map_; }
		//コンオーネント配列の参照
		const auto& component_array()const { return component_array_; }
	private:
		//コンポーネント配列
		std::vector<std::shared_ptr<GameObjectComponent>> component_array_;
		//コピーが簡単なので、基本的にコンポーネント配列へのインデックスでアクセスする。
		//エイリアスによるコンポーネントインデックスマップ
		std::unordered_map<std::string, size_t> alias_idx_map_;
		//タイプによるコンポーネントインデックスマップ<typeindex,<完全探索済みか(false:少なくとも１つは探索済み,true:全て探索済み),idx>>
		mutable std::unordered_map<std::type_index, std::pair<bool, std::vector<std::size_t>>> type_idx_map_;
		//タイプマップにコンポーネントを追加
		void AddComponentToTypeInfoMap_(const std::type_info& ti, size_t idx);
		//エイリアスマップにコンポーネントを追加
		bool AddComponentToAliasMap_(const std::string& alias, size_t idx);
	};
}
