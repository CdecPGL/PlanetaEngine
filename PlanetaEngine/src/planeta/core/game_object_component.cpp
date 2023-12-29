#include "game_object_component.hpp"
#include "game_object_component_setup_data.hpp"
#include "log_utility.hpp"
#include "i_scene_internal.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(game_object_component) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(game_object_component, labels)
			.shallow_copy_target(&game_object_component::labels_);
	}

	void game_object_component::initialize() {
		on_initialized();
		is_valid_ = true;
	}

	void game_object_component::finalize() {
		on_finalized();
		is_valid_ = false;
	}

	void game_object_component::activate() {
		if (is_active_) {
			PE_LOG_WARNING("すでに有効なゲームオブジェクトコンポーネントを有効化しようとしました。type:", typeid(*this).name());
			return;
		}
		on_activated();
		is_active_ = true;
	}

	void game_object_component::in_activate() {
		if (!is_active_) {
			PE_LOG_WARNING("すでに無効なゲームオブジェクトコンポーネントを無効化しようとしました。type:", typeid(*this).name());
			return;
		}
		on_inactivated();
		is_active_ = false;
	}

	bool game_object_component::set_scene_and_holder_go_data(const private_::game_object_component_set_up_data &resister_data) {
		game_object_.reset(resister_data.holder_game_object);
		scene_ = resister_data.scene_internal_interface;
		set_scene_internal_interface(resister_data.scene_internal_interface);
		return true;
	}

	i_game_object &game_object_component::game_object() const { return *game_object_; }

	const std::unordered_set<std::string> &game_object_component::labels() const { return labels_; }

	void game_object_component::labels(const std::unordered_set<std::string> &l) { labels_ = l; }

	i_scene &game_object_component::scene() const { return *scene_; }
}
