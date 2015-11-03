#pragma once

#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GameProcessManagerAccessorForGameObject final {
		public:
			explicit GameProcessManagerAccessorForGameObject(GameProcessManagerPublicInterface& gpmpi) :game_process_manager_public_interface_(gpmpi) {};
			/**
			* @brief �Q�[���v���Z�X���쐬����
			* @param (priority) �v���Z�X�D��x
			* @return �쐬�����v���Z�X��WeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) { return game_process_manager_public_interface_.CreateGameProcess<C>(priority); }
			/*�Q�[���v���Z�X��ID����擾����*/
			utility::WeakPointer<GameProcess> GetGameProcess(int id) { return game_process_manager_public_interface_.GetGameProcess(id); }
			/*�Q�[���v���Z�X��ID�ō폜����*/
			bool RemoveGameProcess(int id) { return game_process_manager_public_interface_.RemoveGameProcess(id); };
			/*�V�X�e���v���Z�X���^����擾����*/
			template<class C>
			utility::WeakPointer<C> GetSystemProcess()const { return game_process_manager_public_interface_.GetSystemProcess<C>(); }
			/*�V�X�e���v���Z�X�𖼑O����擾����*/
			utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name) { return game_process_manager_public_interface_.GetSystemProcess(name); }
		private:
			GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}
