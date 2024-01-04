#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace plnt {
	/*衝突マトリックス。初期値はfalse*/
	class collision_group_matrix {
	public:
		//衝突グループを追加
		void add_collision_group(const std::string &col_group_name);
		//衝突グループをまとめて追加
		void add_collision_groups(const std::vector<std::string> &col_group_names);
		//グループ間の衝突可否を設定
		bool set_collision_flag(const std::string &col_group_name_1, const std::string &col_group_name_2, bool flag);
		//グループ間の衝突可否を取得(設定されていない、存在しない場合はfalse)
		[[nodiscard]] bool get_collision_flag(const std::string &col_group_name_1, const std::string &col_group_name_2) const;
		//衝突するペアのリストを取得
		[[nodiscard]] std::vector<std::pair<std::string, std::string>> get_collisionable_group_pair_list() const;
		//衝突グループのリストを取得
		[[nodiscard]] std::vector<std::string> get_collision_group_list() const;

	private:
		std::unordered_map<std::string, std::unordered_map<std::string, bool>> collision_matrix_;
	};
}
