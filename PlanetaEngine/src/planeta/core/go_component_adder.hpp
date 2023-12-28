#pragma once

#include "game_object_component.hpp"

namespace plnt {
	/*! @brief ゲームオブジェクトコンポーネントを追加するためのクラス

	GameObject::AddComponentsProcの引数として渡される。
	*/
	namespace private_ {
		class go_component_adder {
		public:
			explicit go_component_adder(game_object_component_holder &com_holder) noexcept;
			/*! @brief ゲームオブジェクトコンポーネントをIDで追加し名前を付ける

				該当する型がシステムに登録されている必要がある。
			*/
			[[nodiscard]] std::shared_ptr<game_object_component> create_and_add_component(const std::string &com_type_id) const;
			/*! @brief ゲームオブジェクトコンポーネントを型で追加し名前を付ける
			*/
			template <class ComT>
			std::shared_ptr<ComT> create_and_add_component() {
				static_assert(std::is_base_of_v<game_object_component, ComT>,
				              "ComT must derive GAmeObjectComponent.");
				auto com = std::make_shared<ComT>();
				if (!AddComponentToHolder_(com, typeid(ComT))) { return nullptr; }
				return com;
			}

		private:
			game_object_component_holder &com_holder_;
			[[nodiscard]] bool add_component_to_holder(const std::shared_ptr<game_object_component> &com,
			                                           const std::type_info &t_info) const noexcept;
		};
	}
}
