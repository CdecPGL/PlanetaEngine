#include "GOComponentAdder.h"
#include "GameObjectComponentHolder.h"
#include "Reflection.h"
#include "PrefixUtility.h"
#include "ReflectionExceptions.h"

namespace planeta {

	GOComponentAdder::GOComponentAdder(GameObjectComponentHolder& com_holder) :com_holder_(com_holder) {}

	util::NonOwingPointer<GameObjectComponent> GOComponentAdder::CreateAndAddComponent(const std::string& com_id, const std::string& alias) {
		//�I�u�W�F�N�gID���擾���A�R���|�[�l���g�쐬
		auto obj_id = core::AddPrefix(com_id, core::ObjectCategory::GameObjectComponent);
		auto com = Reflection::CreateObjectByObjectTypeID<GameObjectComponent>(obj_id);
		if (com) {
			decltype(auto) tinfo = Reflection::GetStdTypeInfoByObjectTypeID(obj_id);
			if (AddComponentToHolder_(com, tinfo, alias)) {
				return com;
			} else {
				return nullptr;
			}
		} else {
			PE_LOG_ERROR("GameObjectComponent(ObjectTypeID:\"", obj_id, "\"���쐬�ł��܂���ł����B");
			return nullptr;
		}
	}

	util::NonOwingPointer<GameObjectComponent> GOComponentAdder::CreateAndAddComponent(const std::string& com_id) {
		return CreateAndAddComponent(com_id, com_id);
	}

	std::pair<bool, std::string> GOComponentAdder::GetDefaultComID_(const std::type_info& tinfo) {
		try {
			auto id = Reflection::GetObjectTypeIDByStdTypeInfo(tinfo);
			id = core::RemovePrefix(id);
			return{ true, id };
		} catch (reflection_error& e) {
			PE_LOG_ERROR("�R���|�[�l���gID���擾�ł��܂���ł����B�Q�[���I�u�W�F�N�g�R���|�[�l���g�̌^���o�^����Ă��Ȃ��\��������܂��B(", e.what(), ")");
			return{ false,"" };
		}
	}

	bool GOComponentAdder::AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& tinfo, const std::string& alias) {
		if (com_holder_.RegisterComponent(com, tinfo, alias)) {
			return true;
		} else {
			PE_LOG_ERROR("GameObjectComponent��ǉ��ł��܂���ł����B");
			return false;
		}
	}

}