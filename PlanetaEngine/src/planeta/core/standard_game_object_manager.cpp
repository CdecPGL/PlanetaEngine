#include <ranges>

#include "boost/algorithm/string.hpp"

#include "standard_game_object_manager.hpp"
#include "game_object_base.hpp"
#include "StandardTaskManager.hpp"
#include "log_utility.hpp"
#include "game_object_factory.hpp"
#include "i_debug_manager.hpp"

namespace plnt::private_ {
	standard_game_object_manager::standard_game_object_manager() : id_counter_(0),
	                                                         game_object_factory_(
		                                                         std::make_unique<game_object_factory>()) {};
	standard_game_object_manager::~standard_game_object_manager() = default;

	void standard_game_object_manager::update() { remove_proc(); }

	std::shared_ptr<game_object_base> standard_game_object_manager::create_and_set_up_game_object(
		const std::string &game_object_file_id) {
		auto go = game_object_factory_->get_new_game_object(game_object_file_id, scene_internal_interface());
		return go;
	}

	std::shared_ptr<game_object_base> standard_game_object_manager::create_and_set_up_game_object(
		const std::vector<std::string> &com_type_list) {
		auto go = game_object_factory_->get_new_game_object(com_type_list, scene_internal_interface());
		return go;
	}

	int standard_game_object_manager::register_and_initialize_game_object(const std::shared_ptr<game_object_base> &go) {
		assert(go != nullptr);
		int id = id_counter_++;
		go->set_manager_connection(std::make_unique<game_object_manager_connection>(*this, id));
		if (go->process_initialization() == false) {
			PE_LOG_ERROR("ゲームオブジェクトの初期化に失敗しました。");
			return -1;
		}
		inactive_game_objects_.insert(std::make_pair(id, go));
		return id;
	}

	int standard_game_object_manager::register_and_initialize_game_object(const std::shared_ptr<game_object_base> &go,
	                                                                   const std::string &name) {
		assert(go != nullptr);
		int id = register_and_initialize_game_object(go);
		if (id < -1) { return -1; }
		name_id_map_.emplace(name, id);
		return id;
	}

	weak_pointer<i_game_object>
	standard_game_object_manager::create_game_object(const std::string &game_object_def_file_id) {
		if (auto go = create_and_set_up_game_object(game_object_def_file_id); go != nullptr &&
			register_and_initialize_game_object(go) >= 0) { return go; } else {
			PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObject定義ファイルID:", game_object_def_file_id, ")");
			return nullptr;
		}
	}

	weak_pointer<i_game_object> standard_game_object_manager::create_game_object(
		const std::string &game_object_def_file_id, const std::string &name) {
		if (auto go = create_and_set_up_game_object(game_object_def_file_id); go != nullptr &&
			register_and_initialize_game_object(go, name) >= 0) { return go; } else {
			PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObject定義ファイルID:", game_object_def_file_id, ")");
			return nullptr;
		}
	}

	weak_pointer<i_game_object> standard_game_object_manager::create_game_object_with_component_type_id_list(
		const std::vector<std::string> &game_object_component_type_id_list) {
		if (auto go = create_and_set_up_game_object(game_object_component_type_id_list); go != nullptr &&
			register_and_initialize_game_object(go) >= 0) { return go; } else {
			PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(コンポーネントの指定による作成。指定されたコンポーネントTypeIDは:",
			             boost::algorithm::join(game_object_component_type_id_list, "."), ")");
			return nullptr;
		}
	}

	weak_pointer<i_game_object> standard_game_object_manager::create_game_object_with_component_type_id_list(
		const std::vector<std::string> &game_object_component_type_id_list, const std::string &name) {
		if (auto go = create_and_set_up_game_object(game_object_component_type_id_list); go != nullptr &&
			register_and_initialize_game_object(go, name) >= 0) { return go; } else {
			PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(コンポーネントの指定による作成。指定されたコンポーネントTypeIDは:",
			             boost::algorithm::join(game_object_component_type_id_list, "."), ")");
			return nullptr;
		}
	}

	bool standard_game_object_manager::activate_game_object(const int id) {
		const auto it = inactive_game_objects_.find(id);
		if (it == inactive_game_objects_.end()) { return false; }
		it->second->process_activation();
		active_game_objects_.insert(*it);
		inactive_game_objects_.erase(it);
		return true;
	}

	bool standard_game_object_manager::inactivate_game_object(const int id) {
		const auto it = active_game_objects_.find(id);
		if (it == active_game_objects_.end()) { return false; }
		it->second->process_inactivation();
		inactive_game_objects_.insert(*it);
		active_game_objects_.erase(it);
		return true;
	}

	bool standard_game_object_manager::remove_game_object(const int id) {
		if (auto it = active_game_objects_.find(id); it == active_game_objects_.end()) {
			it = inactive_game_objects_.find(id);
			if (it == inactive_game_objects_.end()) { return false; } else {
				it->second->process_disposal();
				garbage_.push_back(it->second);
				inactive_game_objects_.erase(it);
				return true;
			}
		} else {
			it->second->process_inactivation(); //アクティブリストにあったゲームオブジェクトは、無効化処理を行ってから破棄する
			it->second->process_disposal();
			garbage_.push_back(it->second);
			active_game_objects_.erase(it);
			return true;
		}
	}

	void standard_game_object_manager::remove_all_game_objects() {
		for (const auto &go : active_game_objects_ | std::views::values) {
			go->process_inactivation();
			go->process_disposal();
		}
		active_game_objects_.clear();
		for (const auto &go : inactive_game_objects_ | std::views::values) { go->process_disposal(); }
		inactive_game_objects_.clear();
	}

	bool standard_game_object_manager::initialize() { return true; }

	void standard_game_object_manager::finalize() { remove_all_game_objects(); }

	void standard_game_object_manager::remove_proc() { garbage_.clear(); }

	void standard_game_object_manager::debug_information_add_handle(i_debug_information_adder &di_adder) {
		di_adder.add_line("-----GameObjectManager-----");
		di_adder.add_line_v("活動中ゲームオブジェクト数:", active_game_objects_.size());
		di_adder.add_line_v("停止中ゲームオブジェクト数:", inactive_game_objects_.size());
	}
}
