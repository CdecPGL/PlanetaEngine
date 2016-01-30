#pragma once
#include <memory>
#include <functional>

#include"Object.h"
#include "NonCopyable.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	namespace game {
		class IGameObjectAccessor;
		class GameObject;
	}
	namespace core{
		struct SceneDataForGameObject;
		struct GameObjectComponentRegistrationData;
	}
	class GameObjectComponent : public core::Object, public utility::SharedPointerInstance<GameObjectComponent>, private utility::NonCopyable<GameObjectComponent>{
	public:
		GameObjectComponent() :id_(-1) {};
		virtual ~GameObjectComponent() = default;
		bool SystemSetUp(const core::GameObjectComponentRegistrationData& resistration_data, const core::SceneDataForGameObject& special_setup_data);

		bool is_valied()const { return is_valied_; }
		bool is_active()const { return is_active_; }
		game::IGameObjectAccessor& game_object() { assert(game_object_ != nullptr); return *game_object_; }
		const game::IGameObjectAccessor& game_object()const { assert(game_object_ != nullptr); return *game_object_; }

		/*�V�X�e���֐�*/
		virtual bool is_no_update()const { return false; } //�X�V�������s��Ȃ���
		bool Initialize();
		bool Activate();
		void Update() { OnUpdated(); }
		bool InActivate();
		void Finalize();
	protected:
		using GameObjectAccessorType = utility::WeakPointer<game::IGameObjectAccessor>;
		SceneAccessorForGameObject& scene() { return *scene_accessor_; }
	private:
		//			static void* operator new(size_t s){ throw utility::BadNewDeleteOperation("Component::operator new is called."); return nullptr; }
		//			static void operator delete(void* p){ throw utility::BadNewDeleteOperation("Component::operator delete is called."); return; }
		//			static void* operator new[](size_t s){throw utility::BadNewDeleteOperation("Component::operator new[] is called."); return nullptr; }
		//			static void operator delete[](void* p){throw utility::BadNewDeleteOperation("Component::operator delete[] is called."); return; }

		bool is_valied_ = false;
		bool is_active_ = false;
		int id_;

		utility::WeakPointer<game::IGameObjectAccessor> game_object_;
		utility::WeakPointer<SceneAccessorForGameObject> scene_accessor_;

		/*���ʐݒ�֐�*/
		virtual bool SpecialSetUp(const core::SceneDataForGameObject& setup_data) = 0;

		/*�C�x���g�֐�*/
		virtual bool OnInitialized() { return true; }; //��������Q�[���I�u�W�F�N�g���������ꂽ�Ƃ��ɌĂяo�����(�V�X�e���֐�)
		virtual bool OnActivated() { return true; }; //��������Q�[���I�u�W�F�N�g���V�[���ɓo�^�����Ƃ��ɌĂяo�����(�V�X�e���֐�)
		virtual void OnUpdated() { if (is_no_update()) { throw std::logic_error("Though Update() of NoUpdate component must not be called, it is called."); } } //�}�C�t���[���Ăяo�����(�V�X�e���֐�)
		virtual bool OnInactivated() { return true; } //��������Q�[���I�u�W�F�N�g���V�[������o�^���������Ƃ��ɌĂяo����(�V�X�e���֐�)
		virtual void OnFinalized()noexcept {} //��������Q�[���I�u�W�F�N�g���j�������Ƃ��ɌĂяo�����(�V�X�e���֐�)
	};
}