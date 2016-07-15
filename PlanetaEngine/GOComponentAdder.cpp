#include "GOComponentAdder.h"
#include "GameObjectComponentHolder.h"
#include "Reflection.h"
#include "PrefixUtility.h"
#include "ReflectionExceptions.h"

namespace planeta {

	GOComponentAdder::GOComponentAdder(GameObjectComponentHolder& com_holder, std::vector<std::pair<std::string, std::shared_ptr<GameObjectComponent>>>& id_goc_list_buf)
		:com_holder_(com_holder), alias_goc_list_buf_(id_goc_list_buf) {}

	util::NonOwingPointer<GameObjectComponent> GOComponentAdder::CreateAndAddComponent(const std::string& com_id, const std::string& alias) {
		//オブジェクトIDを取得し、コンポーネント作成
		auto obj_id = core::AddPrefix(com_id, core::ObjectCategory::GameObjectComponent);
		auto com = Reflection::CreateObjectByID<GameObjectComponent>(obj_id);
		//名前コンポーネントリストに追加
		alias_goc_list_buf_.push_back(std::make_pair(alias, com));

		return com;
	}

	util::NonOwingPointer<GameObjectComponent> GOComponentAdder::CreateAndAddComponent(const std::string& com_id) {
		return CreateAndAddComponent(com_id, com_id);
	}

	std::pair<bool, std::string> GOComponentAdder::GetDefaultComID_(const std::type_info& tinfo) {
		try {
			auto id = Reflection::GetTypeIDByStdTypeInfo(tinfo);
			id = core::RemovePrefix(id);
			return{ true, id };
		} catch (reflection_error& e) {
			PE_LOG_ERROR("コンポーネントIDを取得できませんでした。ゲームオブジェクトコンポーネントの型が登録されていない可能性があります。(", e.what(), ")");
			return{ false,"" };
		}
	}

	void GOComponentAdder::AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& tinfo) {
		com_holder_.RegisterComponent(com, tinfo);
	}

}