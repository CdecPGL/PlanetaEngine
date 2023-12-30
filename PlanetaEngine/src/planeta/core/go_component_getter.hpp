#pragma once

#include <vector>
#include <functional>
#include "non_owing_pointer.hpp"

namespace plnt {
	namespace private_ {
		class game_object_component_holder;
	}

	class game_object_component;
	/*! @brief ゲームオブジェクトコンポーネントを取得するためのクラス
		
		GameObjectComponentが自信を所有するゲームオブジェクトのコンポーネントを取得するために使われることを想定し、NonOwingPointerで返す。<br/>
		GameObjectComponent::GetComponentsProcの引数として渡される。
	*/
	class go_component_getter {
	public:
		explicit go_component_getter(const private_::game_object_component_holder &com_holder);
		//! コンポーネントを型で取得する。
		template <class ComT>
		non_owing_pointer<ComT> get_component() const {
			static_assert(std::is_base_of_v<game_object_component, ComT> == true,
			              "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(get_component_by_type_info(typeid(ComT), [](game_object_component *goc) {
				return dynamic_cast<ComT *>(goc) !=
					nullptr;
			}));
		}

		//! コンポーネントを型で全て取得する。
		template <class ComT>
		std::vector<non_owing_pointer<ComT>> get_all_components() const {
			static_assert(std::is_base_of_v<game_object_component, ComT> == true,
			              "ComT must drive GameObjectComponent.");
			auto lst = std::move(get_all_components_by_type_info(typeid(ComT), [](game_object_component *goc) {
				return dynamic_cast<ComT *>(goc) != nullptr;
			}));
			std::vector<non_owing_pointer<ComT>> out;
			for (auto &&com : lst) { out.push_back(std::static_pointer_cast<ComT>(com)); }
			return std::move(out);
		}

		//! 条件を満たすコンポーネントを型で全て取得する。
		template <class ComT>
		std::vector<non_owing_pointer<ComT>> get_all_components_with_condition(
			const std::function<bool(const ComT &)> cond) const {
			static_assert(std::is_base_of_v<game_object_component, ComT> == true,
			              "ComT must drive GameObjectComponent.");
			auto lst = std::move(get_all_components_by_type_info(typeid(ComT), [](game_object_component *goc) {
				return dynamic_cast<ComT *>(goc) != nullptr;
			}));
			std::vector<non_owing_pointer<ComT>> out;
			for (auto &&com : lst) {
				if (cond(static_cast<ComT &>(*com))) { out.push_back(std::static_pointer_cast<ComT>(com)); }
			}
			return std::move(out);
		}

	private:
		const private_::game_object_component_holder &com_holder_;
		std::vector<std::shared_ptr<game_object_component>> get_all_components_by_type_info(
			const std::type_info &ti, const std::function<bool(game_object_component *goc)> &type_checker) const;
		std::shared_ptr<game_object_component> get_component_by_type_info(const std::type_info &ti,
		                                                             const std::function<bool(game_object_component *goc)>
		                                                             &type_checker) const;
	};
}
