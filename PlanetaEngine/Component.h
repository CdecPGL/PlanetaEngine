#pragma once
#include <memory>
#include <functional>
#include"Object.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class ISceneAccessForGameObject;
	}
	namespace game{
		class GameObject;
		class IGameObjectAccessor;
		class Component : public core::Object , public utility::SharedPointerInstance<Component>
		{
			friend class GameObject;
		public:
			Component() :_id(-1){};
			virtual ~Component() = default;
			void SetGameObject(const utility::WeakPointer<IGameObjectAccessor>& game_object, int id) { _game_object = game_object; _id = id; }
			void SetSceneAccessor(const utility::WeakPointer<core::ISceneAccessForGameObject>& scene) { _scene = scene; }

			//GameObject�̕������ɌĂ΂��(�V�X�e���֐�)
//			virtual std::shared_ptr<Component> Clone(GameObject& go,std::vector<std::shared_ptr<Component>*>& need_update_pointer_list); //�ʂ̃Q�[���I�u�W�F�N�g�̃R���|�[�l���g�Ƃ��Ď��g�̕������쐬(���̍ۑ������ɁA�X�V���K�v�ȃR���|�[�l���g���X�g���n�����̂ŁA�K�v�Ȃ�o�^����)
			

			bool is_valied()const{ return _is_valied; }
			utility::WeakPointer<IGameObjectAccessor> game_object()const { return _game_object; }
		protected:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
			utility::WeakPointer<core::ISceneAccessForGameObject> scene() { return _scene; }
		private:
			Component(const Component&) = delete;
			Component(Component&&) = delete;
			Component& operator=(const Component&) = delete;
			Component& operator=(Component&&) = delete;

//			static void* operator new(size_t s){ throw utility::BadNewDeleteOperation("Component::operator new is called."); return nullptr; }
//			static void operator delete(void* p){ throw utility::BadNewDeleteOperation("Component::operator delete is called."); return; }
//			static void* operator new[](size_t s){throw utility::BadNewDeleteOperation("Component::operator new[] is called."); return nullptr; }
//			static void operator delete[](void* p){throw utility::BadNewDeleteOperation("Component::operator delete[] is called."); return; }

//			bool _update_component_pointer(std::shared_ptr<Component>*); //�R���|�[�l���g�������|�C���^���R���|�[�l���gID�����ƂɁA���݂̃Q�[���I�u�W�F�N�g����擾����

			bool _is_valied;
			int _id;

			utility::WeakPointer<IGameObjectAccessor> _game_object;
			utility::WeakPointer<core::ISceneAccessForGameObject> _scene;

			/*�V�X�e���֐�*/
			virtual bool NoUpdate_()const { return false; } //�X�V�������s��Ȃ���

			virtual bool Initialize_() { return true; }; //��������Q�[���I�u�W�F�N�g���������ꂽ�Ƃ��ɌĂяo�����(�V�X�e���֐�)
			virtual bool Activated_() { return true; }; //��������Q�[���I�u�W�F�N�g���V�[���ɓo�^�����Ƃ��ɌĂяo�����(�V�X�e���֐�)
			virtual void Update_() { if (NoUpdate_()) { throw std::logic_error("Though Update() of NoUpdate component must not be called, it is called."); } } //�}�C�t���[���Ăяo�����(�V�X�e���֐�)
			virtual bool InActivated_() { return true; } //��������Q�[���I�u�W�F�N�g���V�[������o�^���������Ƃ��ɌĂяo����(�V�X�e���֐�)
			virtual void Finalize_()throw() {} //��������Q�[���I�u�W�F�N�g���j�������Ƃ��ɌĂяo�����(�V�X�e���֐�)
		};
	}
}