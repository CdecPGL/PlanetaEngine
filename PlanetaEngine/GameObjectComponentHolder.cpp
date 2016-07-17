#include "GameObjectComponentHolder.h"
#include "SystemLog.h"

namespace planeta {
	std::shared_ptr<GameObjectComponent> GameObjectComponentHolder::GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker) const {
		auto it = component_type_map_.find(ti);
		if (it == component_type_map_.end()) {
			for (const auto& c : component_list_) {
				//存在したらタイプリストに追加して不完全マークして関数を抜ける
				if (type_checker(c.get())) {
					component_type_map_.emplace(ti, std::make_pair(false, std::vector<std::shared_ptr<GameObjectComponent>>{ c }));
					return c;
				}
			}
			//存在しなかったらタイプリストを空にして完全マークして関数を抜ける
			component_type_map_.emplace(ti, std::make_pair(true, std::vector<std::shared_ptr<GameObjectComponent>>()));
			return nullptr;
		} else {
			const auto& cl = it->second.second;
			return cl.size() > -0 ? cl[0] : nullptr;
		}
	}

	const std::vector<std::shared_ptr<GameObjectComponent>>& GameObjectComponentHolder::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker) const {
		auto it = component_type_map_.find(ti);
		auto all_search = [this, &type_checker, &ti](std::vector<std::shared_ptr<GameObjectComponent>>& t_list) {
			for (const auto& c : component_list_) {
				//存在したらタイプリストに追加
				if (type_checker(c.get())) {
					t_list.push_back(c);
				}
			}
			return t_list;
		};

		if (it == component_type_map_.end()) { //タイプの探索が行われていなかったら
			auto& t_list = component_type_map_.emplace(ti, std::make_pair(true, std::vector<std::shared_ptr<GameObjectComponent>>())).first->second.second;
			all_search(t_list); //全探索する
			return t_list;
		} else {
			if (it->second.first) { //探索が終了していたら
				return it->second.second;
			} else { //部分探索しかしていなかったら
				all_search(it->second.second); //全探索する
				return it->second.second;
			}
		}
	}

	void GameObjectComponentHolder::AddComponentToTypeInfoMap_(const std::type_info& ti, const std::shared_ptr<GameObjectComponent>& com) {
		auto it = component_type_map_.find(ti);
		if (it == component_type_map_.end()) {
			component_type_map_.emplace(ti, std::make_pair(false, std::vector<std::shared_ptr<GameObjectComponent>>({ com })));
		} else {
			assert(component_type_map_[ti].first == false);
			component_type_map_[ti].second.push_back(com);
		}
	}

	bool GameObjectComponentHolder::RegisterComponent(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& tinfo, const std::string& alias) {
		if (!AddComponentToAliasMap_(alias, com)) {
			PE_LOG_ERROR("ゲームオブジェクトコンポーネント(タイプ\"", tinfo.name(), "\",エイリアス\"", alias, "\")の登録に失敗しました。");
			return false;
		}
		component_list_.push_back(com);
		AddComponentToTypeInfoMap_(tinfo, com);
		return true;
	}

	bool GameObjectComponentHolder::AddComponentToAliasMap_(const std::string& alias, const std::shared_ptr<GameObjectComponent>& com) {
		auto it = alias_map_.find(alias);
		if (it == alias_map_.end()) {
			alias_map_.emplace(alias, com);
			return true;
		} else {
			PE_LOG_ERROR("ゲームオブジェクトコンポーネントエイリアス\"", alias, "\"はすでに登録されています。");
			return false;
		}
	}

}