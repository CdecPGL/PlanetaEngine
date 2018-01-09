#include "CollisionGroupMatrix.hpp"
#include "LogUtility.hpp"

namespace planeta {
	void CollisionGroupMatrix::AddCollisionGroup(const std::string& col_group_name) {
		collision_matrix_.emplace(col_group_name, std::unordered_map<std::string, bool>());
		for (auto& col_line : collision_matrix_) {
			if (col_group_name <= col_line.first) {
				collision_matrix_[col_group_name].emplace(col_line.first, false);
			} else {
				collision_matrix_[col_line.first].emplace(col_group_name, false);
			}
		}
	}

	void CollisionGroupMatrix::AddCollisionGroups(const std::vector<std::string>& col_group_names) {
		for (auto& n : col_group_names) {
			AddCollisionGroup(n);
		}
	}

	bool CollisionGroupMatrix::SetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2, bool flag) {
		auto namep = std::minmax(col_group_name_1, col_group_name_2);
		auto it1 = collision_matrix_.find(namep.first);
		if (it1 == collision_matrix_.end()) {
			PE_LOG_ERROR("登録されていない衝突グループ", namep.first, "が指定されました。");
			return false;
		}
		auto it2 = it1->second.find(namep.second);
		if (it2 == it1->second.end()) {
			PE_LOG_ERROR("登録されていない衝突グループ", namep.second, "が指定されました。");
			return false;
		}
		it2->second = flag;
		return true;
	}

	bool CollisionGroupMatrix::GetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2) const {
		auto namep = std::minmax(col_group_name_1, col_group_name_2);
		auto it1 = collision_matrix_.find(namep.first);
		if (it1 == collision_matrix_.end()) {
			PE_LOG_ERROR("登録されていない衝突グループ", namep.first, "が指定されました。");
			return false;
		}
		auto it2 = it1->second.find(namep.second);
		if (it2 == it1->second.end()) {
			PE_LOG_ERROR("登録されていない衝突グループ", namep.second, "が指定されました。");
			return false;
		}
		return it2->second;
	}

	std::vector<std::pair<std::string, std::string>> CollisionGroupMatrix::GetCollisionableGroupPairList() const {
		std::vector<std::pair<std::string, std::string>> out;
		for (const auto& col_line : collision_matrix_) {
			for (const auto& col_elem : col_line.second) {
				if (col_elem.second) {
					out.emplace_back(std::make_pair(col_line.first, col_elem.first));
				}
			}
		}
		return std::move(out);
	}

	std::vector<std::string> CollisionGroupMatrix::GetCollisionGroupList() const {
		std::vector<std::string> out;
		out.reserve(collision_matrix_.size());
		for (const auto& col_line : collision_matrix_) {
			out.push_back(col_line.first);
		}
		return std::move(out);
	}

}