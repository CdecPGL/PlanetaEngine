#pragma once
#include <memory>
#include <functional>
#include"Object.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class SceneAccessorForGameObject;
	}
	namespace game{
		class GameObject;
		class IGameObjectAccessor;
		class GameObjectComponent : public core::Object , public utility::SharedPointerInstance<GameObjectComponent>
		{
			friend class GameObject;
		public:
			GameObjectComponent() :id_(-1){};
			virtual ~GameObjectComponent() = default;
			void SetGameObject(const utility::WeakPointer<IGameObjectAccessor>& game_object, int id) { game_object_ = game_object; id_ = id; }
			void SetSceneAccessor(const utility::WeakPointer<core::SceneAccessorForGameObject>& scene) { scene_accessor_ = scene; }
			

			bool is_valied()const{ return is_valied_; }
			bool is_active()const { return is_active_; }
			IGameObjectAccessor& game_object() { assert(game_object_ != nullptr); return *game_object_; }
			const IGameObjectAccessor& game_object()const { assert(game_object_ != nullptr); return *game_object_; }
		protected:
			using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
			core::SceneAccessorForGameObject& scene() { return *scene_accessor_; }
		private:
			GameObjectComponent(const GameObjectComponent&) = delete;
			GameObjectComponent(GameObjectComponent&&) = delete;
			GameObjectComponent& operator=(const GameObjectComponent&) = delete;
			GameObjectComponent& operator=(GameObjectComponent&&) = delete;

//			static void* operator new(size_t s){ throw utility::BadNewDeleteOperation("Component::operator new is called."); return nullptr; }
//			static void operator delete(void* p){ throw utility::BadNewDeleteOperation("Component::operator delete is called."); return; }
//			static void* operator new[](size_t s){throw utility::BadNewDeleteOperation("Component::operator new[] is called."); return nullptr; }
//			static void operator delete[](void* p){throw utility::BadNewDeleteOperation("Component::operator delete[] is called."); return; }

			bool is_valied_ = false;
			bool is_active_ = false;
			int id_;

			utility::WeakPointer<IGameObjectAccessor> game_object_;
			utility::WeakPointer<core::SceneAccessorForGameObject> scene_accessor_;

			/*�V�X�e���֐�*/
			virtual bool NoUpdate_()const { return false; } //�X�V�������s��Ȃ���
			bool Initialize();
			bool Activate();
			void Update() { Update_(); }
			bool InActivate();
			void Finalize();

			virtual bool Initialize_() { return true; }; //��������Q�[���I�u�W�F�N�g���������ꂽ�Ƃ��ɌĂяo�����(�V�X�e���֐�)
			virtual bool Activated_() { return true; }; //��������Q�[���I�u�W�F�N�g���V�[���ɓo�^�����Ƃ��ɌĂяo�����(�V�X�e���֐�)
			virtual void Update_() { if (NoUpdate_()) { throw std::logic_error("Though Update() of NoUpdate component must not be called, it is called."); } } //�}�C�t���[���Ăяo�����(�V�X�e���֐�)
			virtual bool InActivated_() { return true; } //��������Q�[���I�u�W�F�N�g���V�[������o�^���������Ƃ��ɌĂяo����(�V�X�e���֐�)
			virtual void Finalize_()throw() {} //��������Q�[���I�u�W�F�N�g���j�������Ƃ��ɌĂяo�����(�V�X�e���֐�)
		};
	}
}