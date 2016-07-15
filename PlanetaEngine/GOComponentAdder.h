#pragma once

#include "NonOwingPointer.h"
#include "GameObjectComponent.h"

namespace planeta {
	class GameObjectComponentHolder;
	class GOComponentAdder {
	public:
		GOComponentAdder(GameObjectComponentHolder& com_holder);
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& obj_id, const std::string& com_id);
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& obj_id);

		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent(const std::string& com_id) {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value, "ComT must derive GAmeObjectComponent.");

		}
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent() {
			return CreateAndAddComponent(GetDefaultComID_(typeid(ComT)));
		}


	private:
		GameObjectComponentHolder& com_holder_;
		std::string GetDefaultComID_(const std::type_info& tinfo);
		void AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com,const std::type_info& tinfo);
	};
}
