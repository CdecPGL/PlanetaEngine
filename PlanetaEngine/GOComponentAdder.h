#pragma once

#include <vector>

#include "NonOwingPointer.h"
#include "GameObjectComponent.h"
#include "SystemLog.h"

namespace planeta {
	class GameObjectComponentHolder;
	/*! @brief �Q�[���I�u�W�F�N�g�R���|�[�l���g��ǉ����邽�߂̃N���X

	GameObject::AddConponentsProc�̈����Ƃ��ēn�����B
	*/
	class GOComponentAdder {
	public:
		GOComponentAdder(GameObjectComponentHolder& com_holder);
		/*! @brief �Q�[���I�u�W�F�N�g�R���|�[�l���g��ID�Œǉ������O��t����

			�Y������^���V�X�e���ɓo�^����Ă���K�v������B
			���O�͒�`�t�@�C������̃f�[�^�̃��[�h���A�R���|�[�l���g�Ƀf�[�^��U�蕪���邽�߂Ɏg�p�����B
		*/
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& com_id, const std::string& alias);
		/*! @brief �Q�[���I�u�W�F�N�g�R���|�[�l���g��ID�Œǉ�����

		�Y������^���V�X�e���ɓo�^����Ă���K�v������B
		*/
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& com_id);
		/*! @brief �Q�[���I�u�W�F�N�g�R���|�[�l���g���^�Œǉ������O��t����

		���O�͒�`�t�@�C������̃f�[�^�̃��[�h���A�R���|�[�l���g�Ƀf�[�^��U�蕪���邽�߂Ɏg�p�����B
		*/
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent(const std::string& alias) {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value, "ComT must derive GAmeObjectComponent.");
			auto com = std::make_shared<ComT>();
			if (!AddComponentToHolder_(com, typeid(ComT), alias)) {
				return nullptr;
			}
			return com;
		}
		/*! @brief �Q�[���I�u�W�F�N�g�R���|�[�l���g���^�Œǉ�����

		�V�X�e���ɊY������^���o�^����Ă���K�v������B
		���O�͒�`�t�@�C������̃f�[�^�̃��[�h���A�R���|�[�l���g�Ƀf�[�^��U�蕪���邽�߂Ɏg�p�����B
		*/
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent() {
			auto ret = GetDefaultComID_(typeid(ComT));
			if (ret.first) {
				return CreateAndAddComponent<ComT>(ret.second);
			} else {
				PE_LOG_ERROR("�f�t�H���g�̃R���|�[�l���g�G�C���A�X���擾�ł��܂���ł����B");
				return nullptr;
			}
		}

	private:
		GameObjectComponentHolder& com_holder_;
		std::pair<bool,std::string> GetDefaultComID_(const std::type_info& tinfo);
		bool AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com,const std::type_info& tinfo, const std::string& alias);
	};
}
