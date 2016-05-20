#pragma once

#include <type_traits>
#include <memory>
#include "GameObjectInterface.h"
#include "Delegate.h"
#include "TaskSlot.h"

namespace planeta_engine {
	class Task;
	class GameObjectComponent;
	class IGameObject {
	public:
		//�L����
		virtual bool Activate() = 0;
		//������
		virtual bool Inactivate() = 0;
		//�j��
		virtual void Dispose() = 0;
		//���g��std::shared_ptr���擾
		virtual std::shared_ptr<IGameObject> GetSharedPointer()const = 0;
		//�R���|�[�l���g���^�Ŏ擾����B
		template<class ComT>
		utility::WeakPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		//�w��^�C�v�̃Q�[���I�u�W�F�N�g�C���^�[�t�F�C�X�擾
		template<class GOI>
		std::shared_ptr<GOI> GetInterface()const {
			static_assert(std::is_base_of<GameObjectInterface, GOI>::value == true, "GOI must drive GameObjectInterface.");
			auto ptr = std::dynamic_pointer_cast<GOI>(GetSharedPointer());
			return ptr;
		}
		//�w��^�C�v�̃Q�[���I�u�W�F�N�g�C���^�[�t�F�C�X�������Ă��邩�m�F
		template<class GOI>
		bool HasInterface()const {
			static_assert(std::is_base_of<GameObjectInterface, GOI>::value == true, "GOI must drive GameObjectInterface.");
			return GetInterface<GOI>() != nullptr;
		}
		//�^�X�N���A�^�b�`
		template<class T>
		utility::WeakPointer<T> CreateAndAttachTask(TaskSlot slot);
		//�L�����C�x���g�n���h���o�^
		virtual utility::DelegateConnection AddActivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) = 0;
		//�������C�x���g�n���h���o�^
		virtual utility::DelegateConnection AddInactivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) = 0;
		//�j���C�x���g�n���h���o�^
		virtual utility::DelegateConnection AddDisposedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder) = 0;
	protected:
		virtual std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const = 0;
	};
}
