#pragma once

#include "Object.h"
#include "GameObjectComponentHolder.h"
#include "IGameObjectForComponent.h"
#include "NonCopyable.h"
#include "TaskManagerPublicInterface.h"
#include "NonOwingPointer.h"
#include "GameObjectManagerConnection.h"

namespace planeta {
	class JSONObject;
	namespace core {
		struct SceneData;
	}
	class GameObjectComponent;
	class GOComponentAdder;
	/*! @brief GameObject�̋�̓I�Ȏ������s���N���X�B���ڗp���邱�Ƃ͂Ȃ��B
	*/
	class GameObjectBase :public core::Object, public IGameObjectForComponent, private util::NonCopyable<GameObjectBase>, public std::enable_shared_from_this<GameObjectBase> {
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
		std::shared_ptr<IGameObject> GetSharedPointer() override final;
		//�L�����C�x���g�n���h���o�^
		util::DelegateConnection AddActivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder)override final;
		//�������C�x���g�n���h���o�^
		util::DelegateConnection AddInactivatedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder)override final;
		//�j���C�x���g�n���h���o�^
		util::DelegateConnection AddDisposedEventHandler(util::DelegateHandlerAdder<void>&& hander_adder)override final;

		//�V�X�e���p�֐�(Manager����Ăяo�����GameObject�N���X�ŉB�؂�����)
		//�N���[�����쐬����
		std::shared_ptr<GameObjectBase> Clone();

		//�C���X�^���X�����̏���
		bool ProcessInstantiation();
		//���[�h���̏���
		bool ProcessLoading(const JSONObject& json_object);
		//���������̏���
		bool ProcessInitialization();
		//�L�������̏���
		bool ProcessActivation();
		//���������̏���
		bool ProcessInactivation();
		//�j�����̏���
		bool ProcessDisposal();
		//�}�l�[�W���R�l�N�V�������Z�b�g
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		//�V�[���f�[�^���Z�b�g
		void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data);
	protected:
		//�I�[�o�[���C�h�K�{�֐�
		/*! @brief �R���|�[�l���g�̓o�^���s��
		
			�V�[�����ŁAGameObjectTypeID�AResourceID�̎w�肪���߂Ă̑g�ݍ��킹�ŃQ�[���I�u�W�F�N�g�𐶐������ꍇ�Ɉ�x�����Ăяo�����B<br/>
			������GOComponentAdder�𗘗p���ăR���|�[�l���g�̒ǉ����s���B<br/>
			�����ɃR���|�[�l���g�̐ݒ�����s�����Ƃ��ł��邪�A�Q�[���I�u�W�F�N�g�̃C���X�^���X�Ɉˑ�����ݒ�(this�|�C���^���R���|�[�l���g�ɓn���Ȃ�)�͍s���Ă͂����Ȃ��B<br/>
			�֐��̐擪�ŁA�e�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual void AddComponentsProc(GOComponentAdder& com_adder) = 0;
		//�C�x���g�֐�
		/*! @brief �������C�x���g�֐�

			�Q�[���I�u�W�F�N�g���������ꂽ�Ƃ��ɌĂяo�����B<br/>
			�^�X�N�̍쐬�ȂǁA�Q�[���I�u�W�F�N�g�̏������������s���B�߂�l�͏������̐��ۂ������B<br/>
			�֐��̐擪�ŁA�e�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual bool OnInitialized();
		/*! @brief �L�����C�x���g�֐�
		
			�Q�[���I�u�W�F�N�g���L��������邽�тɌĂяo�����B<br/>
			�Q�[���I�u�W�F�N�g�̗L�����������s���B�߂�l�͗L�����̐��ۂ������B<br/>
			�֐��̐擪�ŁA�e�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual bool OnActivated();
		/*! @brief �������C�x���g�֐�

			�Q�[���I�u�W�F�N�g������������邽�тɌĂяo�����B<br/>
			�Q�[���I�u�W�F�N�g�̖������������s���B�߂�l�͖������̐��ۂ������B<br/>
			�֐��̖����ŁA�e�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual bool OnInactivated();
		/*! @brief �j�����C�x���g�֐�
		
			�Q�[���I�u�W�F�N�g���j������钼�O�ɌĂяo�����B<br/>
			�Q�[���I�u�W�F�N�g�̏I���������s���B�߂�l�͏I�������̐��ۂ������B<br/>
			�֐��̖����ŁA�e�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual bool OnDisposed();
	private:
		//�C���^�[�t�F�C�X�̃I�[�o�[���C�h
		//�Q�[���I�u�W�F�N�g���쐬
		util::WeakPointer<IGameObject> CreateGameObject(const std::string& id, const std::string& resource_id)override final;
		//�Q�[���I�u�W�F�N�g���쐬���ėL����
		util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id, const std::string& resource_id)override final;
		//�f�t�H���g�Q�[���I�u�W�F�N�g���쐬
		util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& id)override final;
		//�f�t�H���g�Q�[���I�u�W�F�N�g���쐬���ėL����
		util::WeakPointer<IGameObject> CreateAndActivateDefaultGameObject(const std::string& id)override final;
		//�R���|�[�l���g���^�Ŏ擾
		std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo_(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		//�R���|�[�l���g���^�ł��ׂĎ擾
		//std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const override final;
		TaskManagerPublicInterface& RefTaskManagerInterface_()override final;
		void SetUpAttachedTask_(TGameObjectOperation& task)override final;

		//�}�l�[�W���R�l�N�V����
		std::unique_ptr<GameObjectManagerConnection> manager_connection_;
		//�V�[���A�N�Z�T
		util::WeakPointer<core::SceneData> scene_data_;
		//�R���|�[�l���g�z���_�[
		GameObjectComponentHolder component_holder_;

		//�R���|�[�l���g�֘A�̏���

		void SetUpGameComponent(GameObjectComponent& com);
		
		//�C�x���g�f���Q�[�g
		util::Delegate<void> activated_event_delegate_;
		util::Delegate<void> inactivated_event_delegate_;
		util::Delegate<void> disposed_event_delegate_;
	};
}
