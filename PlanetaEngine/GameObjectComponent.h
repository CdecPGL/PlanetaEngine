#pragma once
#include <memory>
#include <functional>

#include"Object.h"
#include "NonCopyable.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"
#include "NonOwingPointer.h"

namespace planeta {
	class SceneAccessorForGameObject;
	class JSONObject;
	namespace core{
		struct SceneData;
		struct GameObjectComponentSetUpData;
	}
	class IGameObjectForComponent;
	class GameObjectComponent : public core::Object, public std::enable_shared_from_this<GameObjectComponent>, private util::NonCopyable<GameObjectComponent>{
	public:
		GameObjectComponent() = default;
		virtual ~GameObjectComponent() = default;
		bool SystemSetUp(const core::GameObjectComponentSetUpData& resistration_data);

		bool is_valied()const { return is_valied_; }
		bool is_active()const { return is_active_; }

		/*�V�X�e���֐�(GameObjectBase����Ăяo�����)*/
		bool Load(const JSONObject& json_obj);
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

		util::NonOwingPointer<IGameObjectForComponent> game_object_;

		/*���ʐݒ�֐�*/
		virtual void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data) = 0;

		/*�C�x���g�֐�*/
		virtual bool OnInitialized() { return true; }; //��������Q�[���I�u�W�F�N�g���������ꂽ�Ƃ��ɌĂяo�����(�V�X�e���֐�)
		virtual bool OnActivated() { return true; }; //��������Q�[���I�u�W�F�N�g���V�[���ɓo�^�����Ƃ��ɌĂяo�����(�V�X�e���֐�)
		virtual bool OnInactivated() { return true; } //��������Q�[���I�u�W�F�N�g���V�[������o�^���������Ƃ��ɌĂяo����(�V�X�e���֐�)
		virtual void OnFinalized()noexcept {} //��������Q�[���I�u�W�F�N�g���j�������Ƃ��ɌĂяo�����(�V�X�e���֐�)
	};
}

//GameObejectComponent���V�X�e���ɓo�^����(�^)
#define PE_REGISTER_GAMEOBJECTCOMPONENT(type)\
PE_REGISTER_OBJECT(type,planeta::GameObjectComponent)