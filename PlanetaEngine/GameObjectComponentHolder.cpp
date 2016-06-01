#include "GameObjectComponentHolder.h"

namespace planeta {
	std::shared_ptr<GameObjectComponent> GameObjectComponentHolder::GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker) const {
		auto it = component_type_map_.find(ti);
		if (it == component_type_map_.end()) {
			for (const auto& c : component_list_) {
				//���݂�����^�C�v���X�g�ɒǉ����ĕs���S�}�[�N���Ċ֐��𔲂���
				if (type_checker(c.get())) {
					component_type_map_.emplace(ti, std::make_pair(false, std::vector<std::shared_ptr<GameObjectComponent>>{ c }));
					return c;
				}
			}
			//���݂��Ȃ�������^�C�v���X�g����ɂ��Ċ��S�}�[�N���Ċ֐��𔲂���
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
				//���݂�����^�C�v���X�g�ɒǉ�
				if (type_checker(c.get())) {
					t_list.push_back(c);
				}
			}
			return t_list;
		};

		if (it == component_type_map_.end()) { //�^�C�v�̒T�����s���Ă��Ȃ�������
			auto& t_list = component_type_map_.emplace(ti, std::make_pair(true, std::vector<std::shared_ptr<GameObjectComponent>>())).first->second.second;
			all_search(t_list); //�S�T������
			return t_list;
		} else {
			if (it->second.first) { //�T�����I�����Ă�����
				return it->second.second;
			} else { //�����T���������Ă��Ȃ�������
				all_search(it->second.second); //�S�T������
				return it->second.second;
			}
		}
	}

	void GameObjectComponentHolder::AddComponentToTypeInfoMap(const std::type_info& ti, const std::shared_ptr<GameObjectComponent>& com) {
		auto it = component_type_map_.find(ti);
		if (it == component_type_map_.end()) {
			component_type_map_.emplace(ti, std::make_pair(false, std::vector<std::shared_ptr<GameObjectComponent>>({ com })));
		} else {
			assert(component_type_map_[ti].first == false);
			component_type_map_[ti].second.push_back(com);
		}
	}
}