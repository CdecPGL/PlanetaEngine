#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include"Object.h"
#include "MakeSharedGameObject.h"
#include "IGameObjectAccessor.h"
#include "GameObjectManagerConnection.h"
#include "NonCopyable.h"

namespace planeta_engine {
	class GameObjectComponent;
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	class SceneAccessorForGameObject;
	namespace game {
		class GameObjectAccessorForSetUp; //�������p�֐��g�p����N���X
		//GameObject�N���X(�p���֎~��)
		class GameObject final : public core::Object, public IGameObjectAccessor
		,private utility::NonCopyable<GameObject>{
			friend GameObjectAccessorForSetUp;
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
			utility::WeakPointer<GameObjectComponent> GetComponent(int id) override {
				auto it = component_list_.find(id);
				if (it == component_list_.end()) { return nullptr; } else { return it->second; }
			}
			/*���[�J���g�����X�t�H�[���擾*/
			utility::WeakPointer<components::TransformComponent> GetTransformComponent()override { return transform_; }
			const components::TransformComponent& transform()const override { return *transform_; }
			components::TransformComponent& transform()override { return *transform_; }

			////////////�V�X�e���֐�//////////
			//�쐬
			static std::shared_ptr<GameObject> Create();

			/*�V�[���̃|�C���^���Z�b�g*/
			void SetSceneAccessor(const utility::WeakPointer<SceneAccessorForGameObject>& scene) { scene_accessor_ = scene; }
			//���g�̃N���[���쐬(�e�q�֌W�̓��Z�b�g�����)
	//			std::shared_ptr<GameObject> Clone()const;

				//�X�V(�}�C�t���[���Ă΂��)[�Ăяo����:GameObjectUpdateProcess::Update]
			void Update() {
				UpdateComponent_();
			};
			class GameObjectManagementProxy; //�Ǘ��p�֐��g�p����N���X
			void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mc) { manager_connection_ = std::move(mc); }
			//�������֘A�֐�[�Ăяo����:GameObjectSetUpper]
			bool SetUpSystemComponent();
			bool Initialize_() { return  InitializeComponent_(); };
			//�V�X�e���ɌĂ΂��֐��Q
			//�V�[���o�^���ɌĂ΂��[�Ăяo����:GameObjectManager::Activate]
			bool Activated_() { activated_event(); return ActivateComponent_(); }
			//�V�[���o�^�������ɌĂ΂��[�Ăяo����:GameObjectManager::InActivate]
			bool InActivated_() { inactivated_event(); return InactivateComponent_(); }
			//�I������(�j�����ɌĂ΂��)[�Ăяo����:GameObjectManager::Remove]
			bool Finalize_()throw() { disposed_event(); return FinalizeComponent_(); };

			/*��Q�Ƃ��擾*/
			utility::WeakPointer<IGameObjectAccessor> GetWeakPointer()override { return me(); };
		protected:
			std::shared_ptr<GameObject> me() { return me_.lock(); } //�����̃X�}�|�擾
		private:
			//���ڐ����A�R�s�[�̋֎~
			//����̃R���X�g���N�^
			GameObject();
			~GameObject();

			//			static void* operator new(size_t s){ return Object::operator new(s); }
			//			static void operator delete(void* p){ return Object::operator delete(p); }
			//			static void* operator new[](size_t s){ return Object::operator new[](s); }
			//			static void operator delete[](void* p){ return Object::operator delete[](p); }

			utility::WeakPointer<SceneAccessorForGameObject> scene_accessor_;
			std::unique_ptr<GameObjectManagerConnection> manager_connection_;

			std::weak_ptr<GameObject> me_; //�����̃X�}�[�g�|�C���^
			std::unordered_map<int, std::shared_ptr<GameObjectComponent>> component_list_; //�R���|�[�l���g���X�g
			std::vector<std::shared_ptr<GameObjectComponent>> component_update_list_; //�R���|�[�l���g�X�V���X�g

			mutable std::unordered_map<std::type_index, std::pair<bool, std::vector<std::shared_ptr<GameObjectComponent>>>> component_type_map_; //�^�C�v�ɂ��R���|�[�l���g�}�b�v<typeindex,<���S�T���ς݂�(false:���Ȃ��Ƃ��P�͒T���ς�,true:�S�ĒT���ς�),�R���|�[�l���g���X�g>>
			void AddComponentToTypeInfoMap(const std::type_info& ti,const std::shared_ptr<GameObjectComponent>& com);
			std::shared_ptr<GameObjectComponent> GetComponentByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const override;
			const std::vector<std::shared_ptr<GameObjectComponent>>& GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent*)>& type_checker)const override;


			//////////����R���|�[�l���g//////////
			std::shared_ptr<components::TransformComponent> transform_; //���[�J���`����

			bool is_active_;
			/*�Q�[���I�u�W�F�N�g�R���|�[�l���g�ɑ΂���ꊇ����*/
			bool InitializeComponent_();
			void UpdateComponent_();
			bool FinalizeComponent_();
			bool ActivateComponent_();
			bool InactivateComponent_();
			/*�Q�[���I�u�W�F�N�g�R���|�[�l���g��ǉ�����*/
			template<class C>
			utility::WeakPointer<C> add_component_() {
				static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "C is not derived Component.");
				auto ptr = C::MakeShared<C>();
				SystemSetUpComponent_(*ptr);
				component_list_.insert(std::make_pair(component_id_counter_++, ptr));
				//�X�V�������s���ꍇ�͍X�V���X�g�ɓo�^
				std::shared_ptr<GameObjectComponent> com_ptr = ptr;
				if (!com_ptr->is_no_update()) { component_update_list_.push_back(ptr); }
				//�^�C�v���X�g�ɒǉ�����
				AddComponentToTypeInfoMap(typeid(C), com_ptr);
				return ptr;
			}
			/*�R���|�[�l���g�̃V�X�e���ݒ���s��*/
			void SystemSetUpComponent_(GameObjectComponent& com);
			int component_id_counter_;
			struct create_helper;
		};
		struct GameObject::create_helper {
			GameObject x;
			// �N���X���N���X���ƃe���v���[�g�ɂł���̂Ńe���v���[�g�ŏ����Ă݂�
			template<class... Args>
			explicit create_helper(Args&&... args)
				: x(std::forward<Args>(args)...) {}
			~create_helper() {}
		};
	}
}
