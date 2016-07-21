#include "GOComponentGetter.h"
#include "GameObjectComponentHolder.h"

namespace planeta {

	GOComponentGetter::GOComponentGetter(const GameObjectComponentHolder& com_holder) :com_holder_(com_holder) {}

	std::vector<std::shared_ptr<GameObjectComponent>> GOComponentGetter::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		return std::move(com_holder_.GetAllComponentsByTypeInfo(ti, type_checker));
	}

	std::shared_ptr<GameObjectComponent> GOComponentGetter::GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		return com_holder_.GetComponentByTypeInfo(ti, type_checker);
	}

}