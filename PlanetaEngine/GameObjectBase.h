#pragma once

#include "GameObjectComponentHolder.h"
#include "IGameObject.h"
#include "NonCopyable.h"

namespace planeta_engine {
	class GameObjectComponent;
	class GameObjectBase : public IGameObject,private utility::NonCopyable<GameObjectBase> {
	public:
		//�C���^�[�t�F�C�X�̃I�[�o�[���C�h
		//�L��������
		bool Activate()override final;
		//����������
		bool Inactivate()override final;
		//�j������
		void Dispose()override final;
		//������std::shared_ptr���擾����B
		std::shared_ptr<IGameObject> GetSharedPointer()const override final;

		//�V�X�e���p�֐�(Manager����Ăяo�����)
		//�L�������̏���
		bool ProcessActivation();
		//���������̏���
		bool ProcessInactivation();
		//�j�����̏���
		bool ProcessDisposDisposal();

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
	private:
		//���g�̎�Q��
		std::weak_ptr<GameObjectBase> this_weak_ptr_;
		//�R���|�[�l���g�z���_�[
		GameObjectComponentHolder component_holder_;

		//�R���|�[�l���g�֘A�̏���
		

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