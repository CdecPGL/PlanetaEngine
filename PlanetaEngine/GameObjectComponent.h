#pragma once
#include <memory>
#include <functional>

#include"Object.h"
#include "NonCopyable.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"
#include "NonOwingPointer.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	namespace core{
		struct SceneData;
		struct GameObjectComponentSetUpData;
	}
	class IGameObjectForComponent;
	class GameObjectComponent : public core::Object, public utility::SharedPointerInstance<GameObjectComponent>, private utility::NonCopyable<GameObjectComponent>{
	public:
		GameObjectComponent() = default;
		virtual ~GameObjectComponent() = default;
		bool SystemSetUp(const core::GameObjectComponentSetUpData& resistration_data);

		bool is_valied()const { return is_valied_; }
		bool is_active()const { return is_active_; }

		/*�V�X�e���֐�*/
		bool Initialize();
		bool Activate();
		bool InActivate();
		void Finalize();
	protected:
		//�ێ�����Ă���Q�[���I�u�W�F�N�g�ւ̎Q��
		IGameObjectForComponent& game_object();
	private:
		bool is_valied_ = false;
		bool is_active_ = false;

		utility::NonOwingPointer<IGameObjectForComponent> game_object_;

		/*���ʐݒ�֐�*/
		virtual void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data) = 0;

		/*�C�x���g�֐�*/
		virtual bool OnInitialized() { return true; }; //��������Q�[���I�u�W�F�N�g���������ꂽ�Ƃ��ɌĂяo�����(�V�X�e���֐�)
		virtual bool OnActivated() { return true; }; //��������Q�[���I�u�W�F�N�g���V�[���ɓo�^�����Ƃ��ɌĂяo�����(�V�X�e���֐�)
		virtual bool OnInactivated() { return true; } //��������Q�[���I�u�W�F�N�g���V�[������o�^���������Ƃ��ɌĂяo����(�V�X�e���֐�)
		virtual void OnFinalized()noexcept {} //��������Q�[���I�u�W�F�N�g���j�������Ƃ��ɌĂяo�����(�V�X�e���֐�)
	};
}