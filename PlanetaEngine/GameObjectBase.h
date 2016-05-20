#pragma once

#include "GameObjectComponentHolder.h"
#include "IGameObject.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"

namespace planeta_engine {
	namespace core {
		class SceneData;
	}
	class GameObjectComponent;
	class GameObjectManagerConnection;
	class GameObjectBase : public IGameObject,private utility::NonCopyable<GameObjectBase> {
	public:
		GameObjectBase();
		~GameObjectBase();

		//�C���^�[�t�F�C�X�̃I�[�o�[���C�h
		//�L��������
		bool Activate()override final;
		//����������
		bool Inactivate()override final;
		//�j������
		void Dispose()override final;
		//������std::shared_ptr���擾����B
		std::shared_ptr<IGameObject> GetSharedPointer()const override final;
		//�L�����C�x���g�n���h���o�^
		utility::DelegateConnection AddActivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;
		//�������C�x���g�n���h���o�^
		utility::DelegateConnection AddInactivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;
		//�j���C�x���g�n���h���o�^
		utility::DelegateConnection AddDisposedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;

		//�V�X�e���p�֐�(Manager����Ăяo�����)
		//�L�������̏���
		bool ProcessActivation();
		//���������̏���
		bool ProcessInactivation();
		//�j�����̏���
		bool ProcessDisposal();
		//�}�l�[�W���R�l�N�V�������Z�b�g
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		//�V�[���f�[�^���Z�b�g
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data);


		//�ÓI�֐�
		//GameObject�̍쐬�֐��B�����̃X�}�[�g�|�C���^��ێ����邽�߂ɂ��̊֐������K�v������B
		template<class T>
		static std::shared_ptr<GameObjectBase> Create() {
			static_assert(std::is_base_of<GameObjectBase, T>::value == true, "T must derive GameOBjectBase");
			auto ptr = std::make_shared<T>();
			ptr->this_weak_ptr_ = ptr;
			return std::move(ptr);
		}
	protected:
		//�R���|�[�l���g���쐬�A�ǉ�����B
		template<class ComT>
		utility::WeakPointer<ComT> CreateAndAddComponent() {
			return component_holder_.CreateAndAddComponent();
		}
		//�R���|�[�l���g���^�Ŏ擾����B
		template<class ComT>
		utility::WeakPointer<ComT> GetComponent() {
			return component_holder_.GetComponent<ComT>();
		}
		//�R���|�[�l���g���^�őS�Ď擾����B
		template<class ComT>
		std::vector<utility::WeakPointer<ComT>> GetAllComponents() {
			return std::move(component_holder_.GetAllComponents<ComT>());
		}
		//�Q�[���I�u�W�F�N�g���쐬
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& id);
		//�Q�[���I�u�W�F�N�g���쐬���ėL����
		utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id);
		//�^�X�N���A�^�b�`
		template<class T>
		utility::WeakPointer<Task> AttachTask(TaskSlot slot);
	private:
		//���g�̎�Q��
		std::weak_ptr<GameObjectBase> this_weak_ptr_;
		//�}�l�[�W���R�l�N�V����
		std::unique_ptr<GameObjectManagerConnection> manager_connection_;
		//�V�[���A�N�Z�T
		utility::WeakPointer<core::SceneData> scene_data_;
		//�R���|�[�l���g�z���_�[
		GameObjectComponentHolder component_holder_;

		//�R���|�[�l���g�֘A�̏���
		
		//�C�x���g�f���Q�[�g
		utility::Delegate<void> activated_event_delegate_;
		utility::Delegate<void> inactivated_event_delegate_;
		utility::Delegate<void> disposed_event_delegate_;

		//�C�x���g�֐�
		//�������C�x���g
		virtual bool OnInitialized();
		//�L�����C�x���g
		virtual bool OnActivated();
		//�������C�x���g
		virtual bool OnInactivated();
		//�j�����C�x���g
		virtual bool OnDisposed();
	};
}