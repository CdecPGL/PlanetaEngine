#pragma once

#include <type_traits>
#include <memory>
#include "GameObjectInterface.h"
#include "TaskManagerPublicInterface.h"
#include "Delegate.h"
#include "TaskSlot.h"

namespace planeta {
	class Task;
	class GameObjectComponent;
	class TGameObjectOperation;
	/*! @brief �Q�[���I�u�W�F�N�g�ւ̃A�N�Z�X��񋟂���C���^�[�t�F�C�X�N���X
	*/
	class IGameObject {
	public:
		//! �Q�[���I�u�W�F�N�g���V�[�����ŗL��������B�߂�l�͗L�����̐��ۂ������B  
		virtual bool Activate() = 0;
		//! �Q�[���I�u�W�F�N�g���V�[�����Ŗ���������B�߂�l�͖������̐��ۂ������B  
		virtual bool Inactivate() = 0;
		//! �Q�[���I�u�W�F�N�g���V�[������j������
		virtual void Dispose() = 0;
		//! ���g��std::shared_ptr���擾����
		virtual std::shared_ptr<IGameObject> GetSharedPointer()= 0;
		//! �R���|�[�l���g���^�Ŏ擾����B���p�֎~�B�ł���Α����GetInterface()���g���B
		template<class ComT>
		util::WeakPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo_(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		/*! @brief �w��^�C�v�̃Q�[���I�u�W�F�N�g�C���^�[�t�F�C�X�擾
			
			�e���v���[�g�����Ŏw�肵���^���A���̎q�N���X�̃C���^�[�t�F�C�X�������Ă���ΕԂ��B�����Ă��Ȃ��ꍇ��nullptr��Ԃ��B
		*/
		template<class GOI>
		std::shared_ptr<GOI> GetInterface() {
			static_assert(std::is_base_of<GameObjectInterface<GOI>, GOI>::value == true, "GOI must drive GameObjectInterface.");
			auto ptr = std::dynamic_pointer_cast<GOI>(GetSharedPointer());
			return ptr;
		}
		/*! @brief �w��^�C�v�̃Q�[���I�u�W�F�N�g�C���^�[�t�F�C�X�������Ă��邩�m�F

			�e���v���[�g�����Ŏw�肵���^���A���̎q�N���X�̃C���^�[�t�F�C�X�������Ă��邩�ǂ�����Ԃ��B
		*/
		template<class GOI>
		bool HasInterface() {
			static_assert(std::is_base_of<GameObjectInterface<GOI>, GOI>::value == true, "GOI must drive GameObjectInterface.");
			return GetInterface<GOI>() != nullptr;
		}
		/*! @brief �^�X�N���A�^�b�`����

			�e���v���[�g�����Ŏw�肵���^�̃^�X�N���쐬���A�A�^�b�`����B
			T��TGameObjectOperation���p�������N���X�B
			�Q�[���I�u�W�F�N�g�̃A�^�b�`���ꂽ�^�X�N�̎����͂��̃Q�[���I�u�W�F�N�g�Ɠ����ɂȂ�A�Q�[���I�u�W�F�N�g�̖������L�����ɍ��킹�Ē�~�A�ĊJ����B
		*/
		template<class T>
		util::WeakPointer<T> CreateAndAttachTask(TaskSlot slot) {
			static_assert(std::is_base_of<TGameObjectOperation, T>::value == true, "T must derive TGameObjectOperation");
			auto task = std::make_shared<T>();
			if (!RefTaskManagerInterface_().RegisterTask(task, slot)) { return nullptr; }
			SetUpAttachedTask_(*task);
			return task;
		}
		//! �L�����C�x���g�n���h����o�^����
		virtual util::DelegateConnection AddActivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) = 0;
		//! �������C�x���g�n���h����o�^����
		virtual util::DelegateConnection AddInactivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) = 0;
		//! �j���C�x���g�n���h����o�^����
		virtual util::DelegateConnection AddDisposedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder) = 0;
	protected:
		virtual std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const = 0;
		virtual TaskManagerPublicInterface& RefTaskManagerInterface_() = 0;
		virtual void SetUpAttachedTask_(TGameObjectOperation& task) = 0;
	};
}
