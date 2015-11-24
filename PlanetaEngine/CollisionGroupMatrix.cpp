#include "CollisionGroupMatrix.h"


/*衝突マトリックスは、例えばグループ数が5の時は*
[0]:[0][1][2][3][4]
[1]:[1][2][3][4]
[2]:[2][3][4]
[3]:[3][4]
[4]:[4]
のように格納する
*/


namespace planeta_engine {
	namespace core {

		int CollisionGroupMatrix::GetCollisionGroupID(const std::string& col_group_name) const {
			auto it = collision_group_id_map_.find(col_group_name);
			return it == collision_group_id_map_.end() ? -1 : it->second;
		}

		void CollisionGroupMatrix::AddCollisionGroup(const std::string& col_group_name) {
			int id = collision_group_id_map_.size();
			collision_group_id_map_.emplace(col_group_name, id);
			collision_matrix_.push_back(std::vector<bool>());
			for (auto& cm : collision_matrix_) { cm.push_back(false); }
		}

		void CollisionGroupMatrix::AddCollisionGroups(const std::vector<std::string>& col_group_names) {
			for (auto& n : col_group_names) {
				int id = collision_group_id_map_.size();
				collision_group_id_map_.emplace(n, id);
			}
			collision_matrix_.resize(collision_matrix_.size() + col_group_names.size());
			for (size_t i = 0; i < collision_matrix_.size();++i) {
				collision_matrix_[i].resize(collision_matrix_.size() - i, false);
			}
		}

		bool CollisionGroupMatrix::SetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2, bool flag) {
			int id1 = GetCollisionGroupID(col_group_name_1);
			int id2 = GetCollisionGroupID(col_group_name_2);
			if (id1 < 0 || id2 < 0) { return false; }
			auto idp = std::minmax(id1, id2);
			assert(idp.first < (int)collision_matrix_.size());
			assert(idp.second - idp.first < (int)collision_matrix_[idp.first].size());
			collision_matrix_[idp.first][idp.second - idp.first] = flag;
			return true;
		}
	}
}