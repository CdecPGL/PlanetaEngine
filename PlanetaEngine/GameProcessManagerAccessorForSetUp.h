#pragma once

#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GameProcessManagerAccessorForSetUp final{
		public:
			explicit GameProcessManagerAccessorForSetUp(GameProcessManagerPublicInterface& gpmpi) :game_process_manager_public_interface_(gpmpi) {};
			/**
			* @brief ゲームプロセスを作成する
			* @param (priority) プロセス優先度
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) { return game_process_manager_public_interface_.CreateGameProcess<C>(priority); }
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority, const std::string& name) { return game_process_manager_public_interface_.CreateGameProcess<C>(priority, name); }
		private:
			GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}
