#pragma once

#include <vector>

#include "NonOwingPointer.hpp"
#include "GameObjectComponent.hpp"
#include "LogUtility.hpp"

namespace planeta {
	class GameObjectComponentHolder;
	/*! @brief ゲームオブジェクトコンポーネントを追加するためのクラス

	GameObject::AddConponentsProcの引数として渡される。
	*/
	namespace private_ {
		class GOComponentAdder {
		public:
			GOComponentAdder(GameObjectComponentHolder& com_holder)noexcept;
			/*! @brief ゲームオブジェクトコンポーネントをIDで追加し名前を付ける

				該当する型がシステムに登録されている必要がある。
			*/
			std::shared_ptr<GameObjectComponent> CreateAndAddComponent(const std::string& com_type_id);
			/*! @brief ゲームオブジェクトコンポーネントを型で追加し名前を付ける
			*/
			template<class ComT>
			std::shared_ptr<ComT> CreateAndAddComponent() {
				static_assert(std::is_base_of<GameObjectComponent, ComT>::value, "ComT must derive GAmeObjectComponent.");
				auto com = std::make_shared<ComT>();
				if (!AddComponentToHolder_(com, typeid(ComT))) {
					return nullptr;
				}
				return com;
			}
		private:
			GameObjectComponentHolder& com_holder_;
			bool AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& tinfo)noexcept;
		};
	}
}
