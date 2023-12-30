#include "go_component_adder.hpp"
#include "game_object_component_holder.hpp"
#include "log_utility.hpp"
#include "planeta/reflection/reflection.hpp"
#include "PrefixUtility.hpp"

namespace plnt::private_ {
	go_component_adder::go_component_adder(game_object_component_holder &com_holder) noexcept : com_holder_(com_holder) { }

	std::shared_ptr<game_object_component> go_component_adder::create_and_add_component(const std::string &com_type_id) const {
		//オブジェクトIDを取得し、コンポーネント作成
		auto obj_id = add_prefix(com_type_id, object_category::game_object_component);
		if (auto com = reflection::reflection::create_object_by_object_type_id<game_object_component>(obj_id)) {
			if (decltype(auto) t_info = reflection::reflection::get_std_type_info_by_object_type_id(obj_id);
				add_component_to_holder(com, t_info)) { return com; }
			return nullptr;
		}
		PE_LOG_ERROR("GameObjectComponent(ObjectTypeID:\"", obj_id, "\"を作成できませんでした。");
		return nullptr;
	}

	bool go_component_adder::add_component_to_holder(const std::shared_ptr<game_object_component> &com,
	                                               const std::type_info &t_info) const noexcept {
		if (com_holder_.register_component(com, t_info)) { return true; }
		PE_LOG_ERROR("GameObjectComponentを追加できませんでした。");
		return false;
	}
}
