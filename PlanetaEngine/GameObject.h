#pragma once

#include"Object.h"
#include "MakeSharedGameObject.h"
#include "IGameObjectAccessor.h"
#include "GameObjectResisterConnection.h"
#include<vector>
#include <unordered_map>

namespace planeta_engine{
	namespace core{
		class SceneAccessorForGameObject;
	}
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	namespace game{
		class GameObjectComponent;
		class GameObjectSetUpProxy; //�������p�֐��g�p����N���X
		class GameObjectSetUpper; //�������N���X
		//GameObject�N���X(�p���֎~��)
		class GameObject final : public core::Object,public IGameObjectAccessor{
			friend GameObjectSetUpProxy;
		public:
			//////////���[�U�A�N�Z�X�\�֐�//////////
			//�L����(�Q�[���V�[���ɓo�^����)
			bool Activate()override;
			bool InActivate()override; //������(�Q�[���V�[������o�^��������)
			/*�j��*/
			void Dispose()override;
			//�A�N�Z�T
			bool is_active()const override { return is_active_; }
			//�w��ID�̃R���|�[�l���g�擾
			utility::WeakPointer<GameObjectComponent> GetComponent(int id) override{
				auto it = component_list_.find(id);
				if (it == component_list_.end()) { return nullptr; }
				else { return it->second; }
			}
			/*����R���|�[�l���g�擾*/
			/*�����n�`�擾*/
			utility::WeakPointer<components::GroundComponent> GetBelongingGroundComponent()override { return belonging_ground_ ? belonging_ground_ : GetDumyGroundComponent_(); }
			const components::GroundComponent& belonging_ground()const override{ return belonging_ground_ ? *belonging_ground_ : *GetDumyGroundComponent_(); }
			components::GroundComponent& belonging_ground()override { return belonging_ground_ ? *belonging_ground_ : *GetDumyGroundComponent_(); }
			/*���[�J���g�����X�t�H�[���擾*/
			utility::WeakPointer<components::TransformComponent> GetTransformComponent()override { return transform_; }
			const components::TransformComponent& transform()const override{ return *transform_; }
			components::TransformComponent& transform()override { return *transform_; }
			/*�����n�`���Z�b�g*/
			void SetBelongingGround(const utility::WeakPointer<components::GroundComponent>& belonging_ground)override { belonging_ground_ = belonging_ground; }

			////////////�V�X�e���֐�//////////
			//�쐬
			static std::shared_ptr<GameObject> Create(GameObjectSetUpper& gosu);

			/*�V�[���̃|�C���^���Z�b�g*/
			void SetSceneAccessor(const utility::WeakPointer<core::SceneAccessorForGameObject>& scene) { scene_accessor_ = scene; }
			//���g�̃N���[���쐬(�e�q�֌W�̓��Z�b�g�����)
//			std::shared_ptr<GameObject> Clone()const;

			//�X�V(�}�C�t���[���Ă΂��)[�Ăяo����:GameObjectUpdateProcess::Update]
			void Update(){
				_update_component();	
			};
			class GameObjectManagementProxy; //�Ǘ��p�֐��g�p����N���X
			//�V�X�e���ɌĂ΂��֐��Q
			//������(�쐬���ɌĂ΂��)[�Ăяo����:GameObjectManager::Resist]
			bool _Initialize(std::unique_ptr<GameObjectResisterConnection>&& rc) { resister_connection_ = std::move(rc); return  _initialize_component(); };
			//�V�[���o�^���ɌĂ΂��[�Ăяo����:GameObjectManager::Activate]
			bool _Activated() { return _activate_component(); }
			//�V�[���o�^�������ɌĂ΂��[�Ăяo����:GameObjectManager::InActivate]
			bool _InActivated() { return _inactivate_component(); }
			//�I������(�j�����ɌĂ΂��)[�Ăяo����:GameObjectManager::Remove]
			bool _Finalize()throw() { return _finalize_component(); };

			/*Root�ƂȂ�g�����X�t�H�[���R���|�[�l���g���擾*/
			static std::shared_ptr<components::TransformComponent> GetRootTransformComponent(bool recreate_flag = false);
			/*��Q�Ƃ��擾*/
			utility::WeakPointer<IGameObjectAccessor> GetWeakPointer()override { return me(); };
		protected:
			std::shared_ptr<GameObject> me(){ return me_.lock(); } //�����̃X�}�|�擾
		private:
			//���ڐ����A�R�s�[�̋֎~
			//����̃R���X�g���N�^
			GameObject();
			~GameObject();
			
			GameObject(const GameObject&) = delete;
			GameObject(GameObject&&) = delete;
			const GameObject& operator=(const GameObject&) = delete;
			const GameObject& operator=(GameObject&&) = delete;

//			static void* operator new(size_t s){ return Object::operator new(s); }
//			static void operator delete(void* p){ return Object::operator delete(p); }
//			static void* operator new[](size_t s){ return Object::operator new[](s); }
//			static void operator delete[](void* p){ return Object::operator delete[](p); }

			utility::WeakPointer<core::SceneAccessorForGameObject> scene_accessor_;
			std::unique_ptr<GameObjectResisterConnection> resister_connection_;

			std::weak_ptr<GameObject> me_; //�����̃X�}�[�g�|�C���^
			std::unordered_map<int,std::shared_ptr<GameObjectComponent>> component_list_; //�R���|�[�l���g���X�g
			const std::unordered_map<int, std::shared_ptr<GameObjectComponent>>& _GetComponentList()const override { return component_list_; }
			std::vector<std::shared_ptr<GameObjectComponent>> component_update_list_; //�R���|�[�l���g�X�V���X�g

			//////////����R���|�[�l���g//////////
			std::shared_ptr<components::TransformComponent> transform_; //���[�J���`����
			utility::WeakPointer<components::GroundComponent> belonging_ground_; //�����n�`(�R���X�g���N�^�Ń_�~�[���Z�b�g�����)

			bool is_active_;

			bool _initialize_component();
			void _update_component();
			bool _finalize_component();
			bool _activate_component();
			bool _inactivate_component();
			template<class C>
			utility::WeakPointer<C> _add_component(){ 
				static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "C is not derived Component.");
				auto ptr = C::MakeShared<C>();
				ptr->SetGameObject(me(), component_id_counter_);
				component_list_.insert(std::make_pair(component_id_counter_++, ptr));
				//�X�V�������s���ꍇ�͍X�V���X�g�ɓo�^
				std::shared_ptr<GameObjectComponent> com_ptr = ptr;
				if (!com_ptr->NoUpdate_()) { component_update_list_.push_back(ptr); }
				return ptr;
			}
			int component_id_counter_;
			std::unique_ptr<GameObjectSetUpProxy> game_object_set_up_proxy_;
			struct create_helper;
			//�Z�b�g�A�b�v(Initialize�܂��ɂ�΂��)[�Ăяo����:GameObject::Create]
			bool _SetUp(GameObjectSetUpper& gosu);

			/*�_�~�[�̒n�`�R���|�[�l���g���擾*/
			static std::shared_ptr<components::GroundComponent> GetDumyGroundComponent_();
		};
		struct GameObject::create_helper {
			GameObject x;
			// �N���X���N���X���ƃe���v���[�g�ɂł���̂Ńe���v���[�g�ŏ����Ă݂�
			template<class... Args>
			explicit create_helper(Args&&... args)
				: x(std::forward<Args>(args)...) {
			}
			~create_helper(){}
		};
	}
}
