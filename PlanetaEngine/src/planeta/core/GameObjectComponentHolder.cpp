#include "GameObjectComponentHolder.hpp"
#include "LogUtility.hpp"
#include "GameObjectComponent.hpp"

namespace plnt {
	namespace private_ {
		std::shared_ptr<GameObjectComponent> GameObjectComponentHolder::GetComponentByTypeInfo(
			const std::type_info &ti, const std::function<bool(GameObjectComponent *)> &type_checker) const {
			auto it = type_idx_map_.find(ti);
			if (it == type_idx_map_.end()) {
				for (size_t idx = 0; idx < component_array_.size(); ++idx) {
					//存在したらタイプリストに追加して不完全マークして関数を抜ける
					if (type_checker(component_array_[idx].get())) {
						type_idx_map_.emplace(ti, std::make_pair(false, std::vector<size_t>{idx}));
						return component_array_[idx];
					}
				}
				//存在しなかったらタイプリストを空にして完全マークして関数を抜ける
				type_idx_map_.emplace(ti, std::make_pair(true, std::vector<size_t>{}));
				return nullptr;
			} else {
				const auto &cl = it->second.second;
				return cl.size() > 0 ? component_array_[cl[0]] : nullptr;
			}
		}

		std::vector<std::shared_ptr<GameObjectComponent>> GameObjectComponentHolder::GetAllComponentsByTypeInfo(
			const std::type_info &ti, const std::function<bool(GameObjectComponent *)> &type_checker) const {
			auto it = type_idx_map_.find(ti);
			auto all_search = [this, &type_checker, &ti](std::vector<size_t> &t_list) {
				for (size_t i = 0; i < component_array_.size(); ++i) {
					//存在したらタイプリストに追加
					if (type_checker(component_array_[i].get())) { t_list.push_back(i); }
				}
				return t_list;
			};
			auto com_idxs_to_com_sps_converter = [this](const std::vector<size_t> &idxs) {
				std::vector<std::shared_ptr<GameObjectComponent>> coms(idxs.size());
				for (size_t i = 0; i < idxs.size(); ++i) { coms[i] = component_array_[idxs[i]]; }
				return std::move(coms);
			};

			if (it == type_idx_map_.end()) {
				//タイプの探索が行われていなかったら
				auto &t_list = type_idx_map_.emplace(ti, std::make_pair(true, std::vector<size_t>{})).first->second.
				                             second;
				all_search(t_list); //全探索する
				return std::move(com_idxs_to_com_sps_converter(t_list));
			} else {
				if (it->second.first) {
					//探索が終了していたら
					return std::move(com_idxs_to_com_sps_converter(it->second.second));
				} else {
					//部分探索しかしていなかったら
					all_search(it->second.second); //全探索する
					return std::move(com_idxs_to_com_sps_converter(it->second.second));
				}
			}
		}

		void GameObjectComponentHolder::AddComponentToTypeInfoMap_(const std::type_info &ti, size_t idx) {
			auto it = type_idx_map_.find(ti);
			if (it == type_idx_map_.end()) {
				type_idx_map_.emplace(ti, std::make_pair(false, std::vector<size_t>({idx})));
			} else {
				assert(type_idx_map_[ti].first == false);
				type_idx_map_[ti].second.push_back(idx);
			}
		}

		bool GameObjectComponentHolder::RegisterComponent(const std::shared_ptr<GameObjectComponent> &com,
		                                                  const std::type_info &tinfo) {
			size_t idx = component_array_.size();
			component_array_.push_back(com);
			AddComponentToTypeInfoMap_(tinfo, idx);
			return true;
		}

		//リフレクションシステムを用いてゲームオブジェクトコンポーネントのコピーを行い、エイリアスと型マップもコピーする。
		void GameObjectComponentHolder::CloneToOtherHolder(GameObjectComponentHolder &com_holder) {
			com_holder.component_array_.resize(component_array_.size());
			for (size_t idx = 0; idx < component_array_.size(); ++idx) {
				auto ncom = std::static_pointer_cast<GameObjectComponent>(component_array_[idx]->ReflectiveClone());
				com_holder.component_array_[idx] = ncom;
			}
			com_holder.type_idx_map_ = type_idx_map_;
		}
	}
}
