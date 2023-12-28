#include <ranges>
#include <algorithm>

#include "collision_group_matrix.hpp"

#include "LogUtility.hpp"

namespace plnt {
	void collision_group_matrix::add_collision_group(const std::string &col_group_name) {
		collision_matrix_.emplace(col_group_name, std::unordered_map<std::string, bool>());
		for (const auto &name : collision_matrix_ | std::views::keys) {
			if (col_group_name <= name) { collision_matrix_[col_group_name].emplace(name, false); } else {
				collision_matrix_[name].emplace(col_group_name, false);
			}
		}
	}

	void collision_group_matrix::add_collision_groups(const std::vector<std::string> &col_group_names) {
		for (auto &n : col_group_names) { add_collision_group(n); }
	}

	bool collision_group_matrix::set_collision_flag(const std::string &col_group_name_1,
	                                                const std::string &col_group_name_2, const bool flag) {
		const auto [min_name, max_name] = std::minmax(col_group_name_1, col_group_name_2);
		const auto it1 = collision_matrix_.find(min_name);
		if (it1 == collision_matrix_.end()) {
			PE_LOG_ERROR("登録されていない衝突グループ", min_name, "が指定されました。");
			return false;
		}
		const auto it2 = it1->second.find(max_name);
		if (it2 == it1->second.end()) {
			PE_LOG_ERROR("登録されていない衝突グループ", max_name, "が指定されました。");
			return false;
		}
		it2->second = flag;
		return true;
	}

	bool collision_group_matrix::get_collision_flag(const std::string &col_group_name_1,
	                                                const std::string &col_group_name_2) const {
		const auto [min_name, max_name] = std::minmax(col_group_name_1, col_group_name_2);
		const auto it1 = collision_matrix_.find(min_name);
		if (it1 == collision_matrix_.end()) {
			PE_LOG_ERROR("登録されていない衝突グループ", min_name, "が指定されました。");
			return false;
		}
		const auto it2 = it1->second.find(max_name);
		if (it2 == it1->second.end()) {
			PE_LOG_ERROR("登録されていない衝突グループ", max_name, "が指定されました。");
			return false;
		}
		return it2->second;
	}

	std::vector<std::pair<std::string, std::string>> collision_group_matrix::get_collisionable_group_pair_list() const {
		std::vector<std::pair<std::string, std::string>> out;
		for (const auto &[col_name, col] : collision_matrix_) {
			for (const auto &[row_name, element] : col) { if (element) { out.emplace_back(col_name, row_name); } }
		}
		return out;
	}

	std::vector<std::string> collision_group_matrix::get_collision_group_list() const {
		std::vector<std::string> out;
		out.reserve(collision_matrix_.size());
		for (const auto &name : collision_matrix_ | std::views::keys) { out.push_back(name); }
		return out;
	}
}
