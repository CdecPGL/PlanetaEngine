#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

namespace planeta_engine {
	namespace core {
		/*衝突マトリックス。初期値はfalse*/
		class CollisionGroupMatrix {
		public:
			//指定したグループIDのペアが衝突可能か確認
			inline bool IsCollision(int col_group_id_1, int col_group_id_2)const {
				auto idp = std::minmax(col_group_id_1, col_group_id_2);
				assert(idp.first < (int)collision_matrix_.size());
				assert(idp.second - idp.first < (int)collision_matrix_[idp.first].size());
				return collision_matrix_[idp.first][idp.second - idp.first];
			}
			//衝突グループ名に割り当てられたIDを取得(-1で割り当てられてない)
			int GetCollisionGroupID(const std::string& col_group_name)const;
			//衝突グループを追加
			void AddCollisionGroup(const std::string& col_group_name);
			//衝突グループをまとめて追加
			void AddCollisionGroups(const std::vector<std::string>& col_group_names);
			//グループ名で衝突マトリックスを設定
			bool SetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2, bool flag);
		private:
			std::unordered_map<std::string, int> collision_group_id_map_;
			std::vector<std::vector<bool>> collision_matrix_;
		};
	}
}