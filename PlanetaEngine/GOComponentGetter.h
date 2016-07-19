#pragma once

#include <vector>
#include <functional>
#include "NonOwingPointer.h"

namespace planeta {
	class GameObjectComponentHolder;
	class GameObjectComponent;
	/*! @brief �Q�[���I�u�W�F�N�g�R���|�[�l���g���擾���邽�߂̃N���X
		
		GameObjectComponet�����M�����L����Q�[���I�u�W�F�N�g�̃R���|�[�l���g���擾���邽�߂Ɏg���邱�Ƃ�z�肵�Autil::NonOwingPointer�ŕԂ��B<br/>
		GameObjectComponent::GetComponentsProc�̈����Ƃ��ēn�����B
	*/
	class GOComponentGetter {
	public:
		GOComponentGetter(const GameObjectComponentHolder& com_holder);
		//! �R���|�[�l���g���^�Ŏ擾����B
		template<class ComT>
		util::NonOwingPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo_(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		//! �R���|�[�l���g���^�őS�Ď擾����B
		template<class ComT>
		std::vector<util::NonOwingPointer<ComT>> GetAllComponents()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			auto lst = std::move(GetAllComponentsByTypeInfo(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
			std::vector<util::NonOwingPointer<ComT>> out();
			for (auto&& com : lst) {
				out.push_back(std::static_pointer_cast<ComT>(com));
			}
			return std::move(lst);
		}
	private:
		const GameObjectComponentHolder& com_holder_;
		std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const;
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const;
	};
}