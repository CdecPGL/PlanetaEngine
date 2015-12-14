#pragma once

#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GameProcessManagerAccessorForGameProcess final {
		public:
			explicit GameProcessManagerAccessorForGameProcess(GameProcessManagerPublicInterface& gpmpi) :game_process_manager_public_interface_(gpmpi) {};
			/**
			* @brief ゲームプロセスを作成する
			* @param (priority) プロセス優先度
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) { return game_process_manager_public_interface_.CreateGameProcess<C>(priority); }
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority,const std::string& name) { return game_process_manager_public_interface_.CreateGameProcess<C>(priority,name); }
			/*システムプロセスを型から取得する*/
			template<class C>
			utility::WeakPointer<C> GetGameProcess()const { return game_process_manager_public_interface_.GetProcess<C>(); }
			/*システムプロセスを名前から取得する*/
			utility::WeakPointer<GameProcess> GetGameProcess(const std::string& name) { return game_process_manager_public_interface_.GetProcess(name); }
		private:
			GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}