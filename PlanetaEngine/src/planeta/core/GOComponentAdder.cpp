﻿#include "GOComponentAdder.hpp"
#include "GameObjectComponentHolder.hpp"
#include "planeta/reflection/reflection.hpp"
#include "PrefixUtility.hpp"
#include "..\reflection\reflection_exceptions.hpp"

namespace plnt {
	namespace private_ {
		GOComponentAdder::GOComponentAdder(GameObjectComponentHolder &com_holder) noexcept : com_holder_(com_holder) { }

		std::shared_ptr<GameObjectComponent> GOComponentAdder::CreateAndAddComponent(const std::string &com_id) {
			//オブジェクトIDを取得し、コンポーネント作成
			auto obj_id = private_::AddPrefix(com_id, private_::ObjectCategory::GameObjectComponent);
			auto com = reflection::reflection::create_object_by_object_type_id<GameObjectComponent>(obj_id);
			if (com) {
				decltype(auto) tinfo = reflection::reflection::get_std_type_info_by_object_type_id(obj_id);
				if (AddComponentToHolder_(com, tinfo)) { return com; } else { return nullptr; }
			} else {
				PE_LOG_ERROR("GameObjectComponent(ObjectTypeID:\"", obj_id, "\"を作成できませんでした。");
				return nullptr;
			}
		}

		bool GOComponentAdder::AddComponentToHolder_(const std::shared_ptr<GameObjectComponent> &com,
		                                             const std::type_info &tinfo) noexcept {
			if (com_holder_.RegisterComponent(com, tinfo)) { return true; } else {
				PE_LOG_ERROR("GameObjectComponentを追加できませんでした。");
				return false;
			}
		}
	}
}
