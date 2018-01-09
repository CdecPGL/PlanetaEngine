#include "GOComponentGetter.hpp"
#include "GameObjectComponentHolder.hpp"
#include "LogUtility.hpp"

namespace planeta {

	GOComponentGetter::GOComponentGetter(const private_::GameObjectComponentHolder& com_holder) :com_holder_(com_holder) {}

	std::vector<std::shared_ptr<GameObjectComponent>> GOComponentGetter::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		return std::move(com_holder_.GetAllComponentsByTypeInfo(ti, type_checker));
	}

	std::shared_ptr<GameObjectComponent> GOComponentGetter::GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		auto com = com_holder_.GetComponentByTypeInfo(ti, type_checker);
		if (com) {
			return com;
		} else {
			PE_LOG_WARNING("ゲームオブジェクトコンポーネント(型:", ti.name(), ")を取得できませんでした。");
			return nullptr;
		}
	}

}