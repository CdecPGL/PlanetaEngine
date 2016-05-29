#pragma once

#include "Object.h"
#include "GameObjectComponentHolder.h"
#include "IGameObjectForComponent.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"
#include "NonOwingPointer.h"
#include "GameObjectManagerConnection.h"

namespace planeta_engine {
	namespace core {
		struct SceneData;
	}
	class GameObjectComponent;
	class GameObjectBase :public core::Object, public IGameObjectForComponent, private utility::NonCopyable<GameObjectBase>, public std::enable_shared_from_this<GameObjectBase> {
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
		std::shared_ptr<IGameObject> GetSharedPointer()override final;
		//�L�����C�x���g�n���h���o�^
		utility::DelegateConnection AddActivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;
		//�������C�x���g�n���h���o�^
		utility::DelegateConnection AddInactivatedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;
		//�j���C�x���g�n���h���o�^
		utility::DelegateConnection AddDisposedEventHandler(utility::DelegateHandlerAdder<void>&& hander_adder)override final;

		//�V�X�e���p�֐�(Manager����Ăяo�����GameObject�N���X�ŉB�؂�����)
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
	protected:
		//�R���|�[�l���g���쐬�A�ǉ�����B
		template<class ComT>
		utility::NonOwingPointer<ComT> CreateAndAddComponent() {
			auto com = component_holder_.CreateAndAddComponent();
			SetUpGameComponent(*com);
			return com;
		}
	private:
		//�C���^�[�t�F�C�X�̃I�[�o�[���C�h
		//�Q�[���I�u�W�F�N�g���쐬
		utility::WeakPointer<IGameObject> CreateGameObject(const std::string& id)override final;
		//�Q�[���I�u�W�F�N�g���쐬���ėL����
		utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id)override final;
		//�R���|�[�l���g���^�Ŏ擾
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		//�R���|�[�l���g���^�ł��ׂĎ擾
		std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		TaskManagerPublicInterface& RefTaskManagerInterface_()override final;
		void SetUpAttachedTask_(TGameObjectOperation& task)override final;

		//�}�l�[�W���R�l�N�V����
		std::unique_ptr<GameObjectManagerConnection> manager_connection_;
		//�V�[���A�N�Z�T
		utility::WeakPointer<core::SceneData> scene_data_;
		//�R���|�[�l���g�z���_�[
		GameObjectComponentHolder component_holder_;

		//�R���|�[�l���g�֘A�̏���

		void SetUpGameComponent(GameObjectComponent& com);
		
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
