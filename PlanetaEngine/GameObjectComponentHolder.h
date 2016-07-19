#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <list>
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta {
	class GameObjectComponent;
	/*GO�R���|�[�l���g���Ǘ�����N���X
	GOC�G�C���A�X�̓Q�[���I�u�W�F�N�g�ɂ���ĕς��̂ŁA�Q�[���I�u�W�F�N�g���ł����A�N�Z�X����ׂ��łȂ��B
	*/
	class GameObjectComponentHolder : private util::NonCopyable<GameObjectComponentHolder>{
	public:
		//�R���|�[�l���g��o�^����
		bool RegisterComponent(const std::shared_ptr<GameObjectComponent>& com, const std::type_info& t_info,const std::string& alias);
		//�^�C�v�ŃR���|�[�l���g���擾
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const;
		//�R���|�[�l���g���^�Ŏ擾����B
		template<class ComT>
		std::shared_ptr<ComT> GetComponent() {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must derive GameComponent.");
			auto ptr = GetComponentByTypeInfo(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; });
			return ptr ? std::static_pointer_cast<ComT>(ptr) : nullptr;
		}
		//�^�C�v�Ɉ�v����R���|�[�l���g�����ׂĎ擾����B
		const std::vector<std::shared_ptr<GameObjectComponent>>& GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const;
		//�R���|�[�l���g���^�őS�Ď擾����B
		template<class ComT>
		std::vector<std::shared_ptr<ComT>> GetAllComponents() {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must derive GameComponent.");
			const auto& go_list = GetAllComponentsByTypeInfo(typeid(ComT), [](GameObjectComponent* com) {return dynamic_cast<C*>(com) != nullptr; });
			std::vector<std::shared_ptr<ComT>> ret_list;
			for (const auto& go : go_list) {
				assert(dynamic_pointer_cast<ComT>(go) != nullptr);
				ret_list.push_back(static_pointer_cast<ComT>(go));
			}
			return std::move(ret_list);
		}
		//�G�C���A�X�}�b�v�̎Q��
		const auto& alias_map()const { return alias_map_; }
		//�R���I�[�l���g���X�g�̎Q��
		const auto& component_list()const { return component_list_; }
	private:
		//�R���|�[�l���g���X�g
		std::list<std::shared_ptr<GameObjectComponent>> component_list_;
		//�G�C���A�X�ɂ��R���|�[�l���g�}�b�v
		std::unordered_map<std::string, std::shared_ptr<GameObjectComponent>> alias_map_;
		//�^�C�v�ɂ��R���|�[�l���g�}�b�v<typeindex,<���S�T���ς݂�(false:���Ȃ��Ƃ��P�͒T���ς�,true:�S�ĒT���ς�),�R���|�[�l���g���X�g>>
		mutable std::unordered_map<std::type_index, std::pair<bool, std::vector<std::shared_ptr<GameObjectComponent>>>> component_type_map_;
		//�^�C�v�}�b�v�ɃR���|�[�l���g��ǉ�
		void AddComponentToTypeInfoMap_(const std::type_info& ti, const std::shared_ptr<GameObjectComponent>& com);
		//�G�C���A�X�}�b�v�ɃR���|�[�l���g��ǉ�
		bool AddComponentToAliasMap_(const std::string& alias, const std::shared_ptr<GameObjectComponent>& com);
	};
}
