#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <vector>

#include "WeakPointer.hpp"
#include "non_copyable.hpp"

namespace plnt {
	class game_object_component;

	namespace private_ {
		/*GOコンポーネントを管理するクラス
		*/
		class game_object_component_holder : util::non_copyable<game_object_component_holder> {
		public:
			//コンポーネントを登録する
			bool register_component(const std::shared_ptr<game_object_component> &com, const std::type_info &t_info);
			//タイプでコンポーネントを取得
			std::shared_ptr<game_object_component> get_component_by_type_info(const std::type_info &ti,
			                                                            const std::function<bool(game_object_component *)>
			                                                            &
			                                                            type_checker) const;
			//コンポーネントを型で取得する。
			template <class ComT>
			std::shared_ptr<ComT> get_component() {
				static_assert(std::is_base_of_v<game_object_component, ComT> == true,
				              "ComT must derive GameComponent.");
				auto ptr = GetComponentByTypeInfo(typeid(ComT), [](game_object_component *goc) {
					return dynamic_cast<ComT *>(goc) != nullptr;
				});
				return ptr ? std::static_pointer_cast<ComT>(ptr) : nullptr;
			}

			//タイプに一致するコンポーネントをすべて取得する。
			std::vector<std::shared_ptr<game_object_component>> get_all_components_by_type_info(
				const std::type_info &ti, const std::function<bool(game_object_component *)> &type_checker) const;
			//コンポーネントを型で全て取得する。
			template <class ComT>
			std::vector<std::shared_ptr<ComT>> get_all_components() {
				static_assert(std::is_base_of_v<game_object_component, ComT> == true,
				              "ComT must derive GameComponent.");
				auto go_list = std::move(GetAllComponentsByTypeInfo(typeid(ComT), [](game_object_component *com) {
					return dynamic_cast<ComT *>(com) != nullptr;
				}));
				std::vector<std::shared_ptr<ComT>> ret_list;
				for (const auto &go : go_list) {
					assert(std::dynamic_pointer_cast<ComT>(go) != nullptr);
					ret_list.push_back(std::static_pointer_cast<ComT>(go));
				}
				return std::move(ret_list);
			}

			void clone_to_other_holder(game_object_component_holder &com_holder) const;
			//コンオーネント配列の参照
			const auto &component_array() const { return component_array_; }

		private:
			//コンポーネント配列
			std::vector<std::shared_ptr<game_object_component>> component_array_;
			//コピーが簡単なので、基本的にコンポーネント配列へのインデックスでアクセスする。
			//タイプによるコンポーネントインデックスマップ<type_index,<完全探索済みか(false:少なくとも１つは探索済み,true:全て探索済み),idx>>
			mutable std::unordered_map<std::type_index, std::pair<bool, std::vector<std::size_t>>> type_idx_map_;
			//タイプマップにコンポーネントを追加
			void add_component_to_type_info_map(const std::type_info &ti, size_t idx) const;
		};
	}
}
