#pragma once

#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GameProcessManagerAccessorForGameProcess final {
		public:
			explicit GameProcessManagerAccessorForGameProcess(GameProcessManagerPublicInterface& gpmpi) :game_process_manager_public_interface_(gpmpi) {};
			/**
			* @brief �Q�[���v���Z�X���쐬����
			* @param (priority) �v���Z�X�D��x
			* @return �쐬�����v���Z�X��WeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) { return game_process_manager_public_interface_.CreateGameProcess<C>(priority); }
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority,const std::string& name) { return game_process_manager_public_interface_.CreateGameProcess<C>(priority,name); }
			/*�V�X�e���v���Z�X���^����擾����*/
			template<class C>
			utility::WeakPointer<C> GetGameProcess()const { return game_process_manager_public_interface_.GetProcess<C>(); }
			/*�V�X�e���v���Z�X�𖼑O����擾����*/
			utility::WeakPointer<GameProcess> GetGameProcess(const std::string& name) { return game_process_manager_public_interface_.GetProcess(name); }
		private:
			GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}