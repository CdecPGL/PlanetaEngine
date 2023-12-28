#include "game_object_component_holder.hpp"
#include "game_object_component.hpp"

namespace plnt::private_ {
	std::shared_ptr<game_object_component> game_object_component_holder::get_component_by_type_info(
		const std::type_info &ti, const std::function<bool(game_object_component *)> &type_checker) const {
		if (const auto it = type_idx_map_.find(ti); it == type_idx_map_.end()) {
			for (size_t idx = 0; idx < component_array_.size(); ++idx) {
				//存在したらタイプリストに追加して不完全マークして関数を抜ける
				if (type_checker(component_array_[idx].get())) {
					type_idx_map_.emplace(ti, std::make_pair(false, std::vector{idx}));
					return component_array_[idx];
				}
			}
			//存在しなかったらタイプリストを空にして完全マークして関数を抜ける
			type_idx_map_.emplace(ti, std::make_pair(true, std::vector<size_t>{}));
			return nullptr;
		} else {
			const auto &cl = it->second.second;
			return !cl.empty() ? component_array_[cl[0]] : nullptr;
		}
	}

	std::vector<std::shared_ptr<game_object_component>> game_object_component_holder::get_all_components_by_type_info(
		const std::type_info &ti, const std::function<bool(game_object_component *)> &type_checker) const {
		const auto it = type_idx_map_.find(ti);
		auto all_search = [this, &type_checker](std::vector<size_t> &t_list) {
			for (size_t i = 0; i < component_array_.size(); ++i) {
				//存在したらタイプリストに追加
				if (type_checker(component_array_[i].get())) { t_list.push_back(i); }
			}
			return t_list;
		};
		auto com_idxes_to_com_sps_converter = [this](const std::vector<size_t> &idxes) {
			std::vector<std::shared_ptr<game_object_component>> coms(idxes.size());
			for (size_t i = 0; i < idxes.size(); ++i) { coms[i] = component_array_[idxes[i]]; }
			return coms;
		};

		if (it == type_idx_map_.end()) {
			//タイプの探索が行われていなかったら
			auto &t_list = type_idx_map_.emplace(ti, std::make_pair(true, std::vector<size_t>{})).first->second.
			                             second;
			all_search(t_list); //全探索する
			return com_idxes_to_com_sps_converter(t_list);
		}
		if (it->second.first) {
			//探索が終了していたら
			return com_idxes_to_com_sps_converter(it->second.second);
		}
		//部分探索しかしていなかったら
		all_search(it->second.second); //全探索する
		return com_idxes_to_com_sps_converter(it->second.second);
	}

	void game_object_component_holder::add_component_to_type_info_map(const std::type_info &ti, size_t idx) const {
		if (const auto it = type_idx_map_.find(ti); it == type_idx_map_.end()) {
			type_idx_map_.emplace(ti, std::make_pair(false, std::vector({idx})));
		} else {
			assert(type_idx_map_[ti].first == false);
			type_idx_map_[ti].second.push_back(idx);
		}
	}

	bool game_object_component_holder::register_component(const std::shared_ptr<game_object_component> &com,
	                                                   const std::type_info &t_info) {
		const size_t idx = component_array_.size();
		component_array_.push_back(com);
		add_component_to_type_info_map(t_info, idx);
		return true;
	}

	//リフレクションシステムを用いてゲームオブジェクトコンポーネントのコピーを行い、エイリアスと型マップもコピーする。
	void game_object_component_holder::clone_to_other_holder(game_object_component_holder &com_holder) const {
		com_holder.component_array_.resize(component_array_.size());
		for (size_t idx = 0; idx < component_array_.size(); ++idx) {
			const auto new_com = std::static_pointer_cast<game_object_component>(component_array_[idx]->reflective_clone());
			com_holder.component_array_[idx] = new_com;
		}
		com_holder.type_idx_map_ = type_idx_map_;
	}
}
