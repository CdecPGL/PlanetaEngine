#include "GOComponentAdder.h"
#include "GameObjectComponentHolder.h"
#include "Reflection.h"
#include "PrefixUtility.h"
#include "ReflectionExceptions.h"

namespace planeta {

	GOComponentAdder::GOComponentAdder(GameObjectComponentHolder& com_holder)noexcept :com_holder_(com_holder) {}

	bool GOComponentAdder::CreateAndAddComponent(const std::string& com_id, const std::string& alias) {
		//オブジェクトIDを取得し、コンポーネント作成
		auto obj_id = private_::AddPrefix(com_id, private_::ObjectCategory::GameObjectComponent);
		auto com = Reflection::CreateObjectByObjectTypeID<GameObjectComponent>(obj_id);
		if (com) {
			decltype(auto) tinfo = Reflection::GetStdTypeInfoByObjectTypeID(obj_id);
			if (AddComponentToHolder_(com, tinfo, alias)) {
				return true;
			} else {
				return false;
			}
		} else {
			PE_LOG_ERROR("GameObjectComponent(ObjectTypeID:\"", obj_id, "\"を作成できませんでした。");
			return false;
		}
	}

	bool GOComponentAdder::CreateAndAddComponent(const std::string& com_id) {
		return CreateAndAddComponent(com_id, com_id);
	}

	std::pair<bool, std::string> GOComponentAdder::GetDefaultComID_(const std::type_info& tinfo)noexcept {
		try {
			auto id = Reflection::GetObjectTypeIDByStdTypeInfo(tinfo);
			id = private_::RemovePrefix(id);
			return{ true, id };
		} catch (reflection_error& e) {
			PE_LOG_ERROR("コンポーネントIDを取得できませんでした。ゲームオブジェクトコンポーネントの型が登録されていない可能性があります。(", e.what(), ")");
			return{ false,"" };
		}
	}

	bool GOComponentAdder::AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& tinfo, const std::string& alias)noexcept {
		if (com_holder_.RegisterComponent(com, tinfo, alias)) {
			return true;
		} else {
			PE_LOG_ERROR("GameObjectComponentを追加できませんでした。");
			return false;
		}
	}

}