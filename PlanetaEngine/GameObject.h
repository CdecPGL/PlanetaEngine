#ifndef DEF_OBJECT_H //��dinclude�h�~

#define DEF_OBJECT_H

#include"Object.h"
//#include "ObjectHolderTemplate_WithoutID.h"
//#include "GameObjectAllocator.h"
#include "MakeSharedGameObject.h"
#include "IGameObjectAccessor.h"
#include<vector>
#include <unordered_map>

namespace planeta_engine{
	namespace core{
		class ISceneAccessForGameObject;
	}
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	namespace game{
		class GameObjectManager;
		class GameProcessManager;
		class Component;
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
			bool is_active()const override { return _is_active; }
			int ID()const override { return _id; }
			//�R���|�[�l���g�֘A
			//����^�̃R���|�[�l���g�擾
			/*template<class C>
			std::shared_ptr<C> GetComponent()const override{
				for (const auto& com : _components) {
					auto ptr = dynamic_pointer_cast<C>(com.second);
					if (ptr != nullptr) { return ptr; }
				}
				return nullptr;
			}*/
			//�w��ID�̃R���|�[�l���g�擾
			utility::WeakPointer<Component> GetComponent(int id) override{
				auto it = _component_list.find(id);
				if (it == _component_list.end()) { return nullptr; }
				else { return it->second; }
			}
			/*����R���|�[�l���g�擾*/
			/*�����n�`�擾*/
			utility::WeakPointer<components::GroundComponent> GetBelongingGroundComponent()override { return _belonging_ground ? _belonging_ground : GetDumyGroundComponent_(); }
			const components::GroundComponent& belonging_ground()const override{ return _belonging_ground ? *_belonging_ground : *GetDumyGroundComponent_(); }
			components::GroundComponent& belonging_ground()override { return _belonging_ground ? *_belonging_ground : *GetDumyGroundComponent_(); }
			/*���[�J���g�����X�t�H�[���擾*/
			utility::WeakPointer<components::TransformComponent> GetTransformComponent()override { return _transform; }
			const components::TransformComponent& transform()const override{ return *_transform; }
			components::TransformComponent& transform()override { return *_transform; }
			/*�����n�`���Z�b�g*/
			void SetBelongingGround(const utility::WeakPointer<components::GroundComponent>& belonging_ground)override { _belonging_ground = belonging_ground; }

			////////////�V�X�e���֐�//////////
			//�쐬
			static std::shared_ptr<GameObject> Create(GameObjectSetUpper& gosu);

			/*�V�[���̃|�C���^���Z�b�g*/
			void SetSceneAccessor(const utility::WeakPointer<core::ISceneAccessForGameObject>& scene) { _scene = scene; }
			//���g�̃N���[���쐬(�e�q�֌W�̓��Z�b�g�����)
//			std::shared_ptr<GameObject> Clone()const;

			//�X�V(�}�C�t���[���Ă΂��)[�Ăяo����:GameObjectUpdateProcess::Update]
			void Update(){
				_update_component();	
			};
			class GameObjectManagementProxy; //�Ǘ��p�֐��g�p����N���X
			//�V�X�e���ɌĂ΂��֐��Q
			//������(�쐬���ɌĂ΂��)[�Ăяo����:GameObjectManager::Resist]
			bool _Initialize(int id) { _id = id; return  _initialize_component(); };
			//�V�[���o�^���ɌĂ΂��[�Ăяo����:GameObjectManager::Activate]
			bool _Activated() { return _activate_component(); }
			//�V�[���o�^�������ɌĂ΂��[�Ăяo����:GameObjectManager::InActivate]
			bool _InActivated() { return _inactivate_component(); }
			//�I������(�j�����ɌĂ΂��)[�Ăяo����:GameObjectManager::Remove]
			bool _Finalize()throw() { return _finalize_component(); };

			/*Root�ƂȂ�g�����X�t�H�[���R���|�[�l���g���擾*/
			static std::shared_ptr<components::TransformComponent> GetRootTransformComponent(bool recreate_flag = false);
		protected:
			std::shared_ptr<GameObject> me(){ return _me.lock(); } //�����̃X�}�|�擾
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

			utility::WeakPointer<core::ISceneAccessForGameObject> _scene;

			std::weak_ptr<GameObject> _me; //�����̃X�}�[�g�|�C���^
			std::unordered_map<int,std::shared_ptr<Component>> _component_list; //�R���|�[�l���g���X�g
			const std::unordered_map<int, std::shared_ptr<Component>>& _GetComponentList()const override { return _component_list; }
			std::vector<std::shared_ptr<Component>> _component_update_list; //�R���|�[�l���g�X�V���X�g

			//////////����R���|�[�l���g//////////
			std::shared_ptr<components::TransformComponent> _transform; //���[�J���`����
			utility::WeakPointer<components::GroundComponent> _belonging_ground; //�����n�`(�R���X�g���N�^�Ń_�~�[���Z�b�g�����)

			bool _is_active;
			int _id; //�Q�[���I�u�W�F�N�gID(-1�Ŗ��o�^)

			bool _initialize_component();
			void _update_component();
			bool _finalize_component();
			bool _activate_component();
			bool _inactivate_component();
			template<class C>
			utility::WeakPointer<C> _add_component(){ 
				static_assert(std::is_base_of<Component, C>::value == true, "C is not derived Component.");
				auto ptr = C::MakeShared<C>();
				ptr->SetGameObject(me(), _component_id_counter);
				_component_list.insert(std::make_pair(_component_id_counter++, ptr));
				//�X�V�������s���ꍇ�͍X�V���X�g�ɓo�^
				std::shared_ptr<Component> com_ptr = ptr;
				if (!com_ptr->NoUpdate_()) { _component_update_list.push_back(ptr); }
				return ptr;
			}
			int _component_id_counter;
			std::unique_ptr<GameObjectSetUpProxy> _game_object_set_up_proxy;
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

#endif