#pragma once

#include <type_traits>
#include "GameObjectBase.h"
#include "GameObjectInterface.h"
#include "MetaprogrammingUtility.h"

namespace planeta_engine {
	//�Q�[���I�u�W�F�N�g���`����Ƃ��́A���̃N���X���p������B�e���v���[�g�����ɂ�GameObjectInterface��C�ӌw�肷��B
	template<typename... GOI>
	class GameObject : public GameObjectBase , public GOI... {
		//GOI...��GameObjectInterface�̔h���N���X���m�F�B
		template<typename Arg> struct GOI_is_base_of :public std::is_base_of<GameObjectInterface, Arg> {};
		static_assert(mp_utiliey::AllOf<GOI_is_base_of, GOI...>::value == false, "GOI must derive GameObjectInterface.");
	public:
		GameObject():GOI(*this)... {}
	private:
		//�V�X�e���֐��̉B��
		bool ProcessActivation();
		bool ProcessInactivation();
		bool ProcessDisposal();
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data);
	};
}

//GameObject���V�X�e���ɓo�^����(�^)
#define PE_REGISTER_GAMEOBJECT(type)\
PE_REGISTER_OBJECT(type,planeta_engine::GameObjectBase)
