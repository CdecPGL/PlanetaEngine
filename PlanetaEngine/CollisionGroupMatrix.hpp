#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

namespace planeta {
	/*衝突マトリックス。初期値はfalse*/
	class CollisionGroupMatrix {
	public:
		//衝突グループを追加
		void AddCollisionGroup(const std::string& col_group_name);
		//衝突グループをまとめて追加
		void AddCollisionGroups(const std::vector<std::string>& col_group_names);
		//グループ間の衝突可否を設定
		bool SetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2, bool flag);
		//グループ間の衝突可否を取得(設定されていない、存在しない場合はfalse)
		bool GetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2)const;
		//衝突するペアのリストを取得
		std::vector<std::pair<std::string, std::string>> GetCollisionableGroupPairList()const;
		//衝突グループのリストを取得
		std::vector<std::string> GetCollisionGroupList()const;
	private:
		std::unordered_map<std::string, std::unordered_map<std::string, bool>> collision_matrix_;
	};
}