#pragma once

#include <type_traits>
#include "GameObjectBase.h"
#include "GameObjectInterface.h"
#include "MetaprogrammingUtility.h"
#include "GOComponentAdder.h"

namespace planeta {
	/*! @brief �Q�[���I�u�W�F�N�g�̊��ƂȂ�N���X�B�Q�[���I�u�W�F�N�g���`����Ƃ��͂��̃N���X���p������B
	
		�e���v���[�g�����ɂ�GameObjectInterface��C�ӌw�肷��B
	*/
	template<typename... GOI>
	class GameObject : public GameObjectBase , public GOI... {
		//GOI...��GameObjectInterface�̔h���N���X���m�F�B
		template<typename Arg> struct GOI_is_base_of :public std::is_base_of<GameObjectInterface<Arg>, Arg> {};
		static_assert(mp_utiliey::AllOf<GOI_is_base_of, GOI...>::value == true, "GOI must derive GameObjectInterface.");
	public:
		//! �K��̃R���X�g���N�^
		GameObject():GOI(*this)... {}
	private:
		//�V�X�e���֐��̉B��
		bool ProcessInstantiation();
		bool ProcessLoading();
		bool ProcessActivation();
		bool ProcessInactivation();
		bool ProcessDisposal();
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data);
	};
}

/*! @def
	GameObject���V�X�e���ɓo�^����
	@param(type) �o�^�������^
*/
#define PE_REGISTER_GAMEOBJECT(type)\
PE_REGISTER_OBJECT(type,planeta::GameObjectBase)
