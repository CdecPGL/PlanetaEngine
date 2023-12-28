#include "go_component_getter.hpp"
#include "game_object_component_holder.hpp"
#include "LogUtility.hpp"

namespace plnt {
	go_component_getter::go_component_getter(const private_::game_object_component_holder &com_holder) : com_holder_(
		com_holder) { }

	std::vector<std::shared_ptr<game_object_component>> go_component_getter::get_all_components_by_type_info(
		const std::type_info &ti, const std::function<bool(game_object_component *goc)> &type_checker) const {
		return com_holder_.get_all_components_by_type_info(ti, type_checker);
	}

	std::shared_ptr<game_object_component> go_component_getter::get_component_by_type_info(
		const std::type_info &ti, const std::function<bool(game_object_component *goc)> &type_checker) const {
		if (auto com = com_holder_.get_component_by_type_info(ti, type_checker)) { return com; }
		PE_LOG_WARNING("ゲームオブジェクトコンポーネント(型:", ti.name(), ")を取得できませんでした。");
		return nullptr;
	}
}
