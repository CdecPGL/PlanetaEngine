#include "GOComponentAdder.h"
#include "GameObjectComponentHolder.h"
#include "Reflection.h"

namespace planeta {

	GOComponentAdder::GOComponentAdder(GameObjectComponentHolder& com_holder) :com_holder_(com_holder) {}

	util::NonOwingPointer<GameObjectComponent> GOComponentAdder::CreateAndAddComponent(const std::string& obj_id, const std::string& com_id) {
		return nullptr;
	}

	std::string GOComponentAdder::GetDefaultComID_(const std::type_info& tinfo) {
		//Reflection::GetTypeIDByType(tinfo)
		return "";
	}

	void GOComponentAdder::AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& tinfo) {
		com_holder_.RegisterComponent(com, tinfo);
	}

}