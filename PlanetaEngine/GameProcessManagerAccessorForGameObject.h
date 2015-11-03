#pragma once

#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace game {
		class GameProcessManagerAccessorForGameObject final {
		public:
			explicit GameProcessManagerAccessorForGameObject(GameProcessManagerPublicInterface& gpmpi) :game_process_manager_public_interface_(gpmpi) {};
			/**
			* @brief ゲームプロセスを作成する
			* @param (priority) プロセス優先度
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) { return game_process_manager_public_interface_.CreateGameProcess<C>(priority); }
			/*ゲームプロセスをIDから取得する*/
			utility::WeakPointer<GameProcess> GetGameProcess(int id) { return game_process_manager_public_interface_.GetGameProcess(id); }
			/*ゲームプロセスをIDで削除する*/
			bool RemoveGameProcess(int id) { return game_process_manager_public_interface_.RemoveGameProcess(id); };
			/*システムプロセスを型から取得する*/
			template<class C>
			utility::WeakPointer<C> GetSystemProcess()const { return game_process_manager_public_interface_.GetSystemProcess<C>(); }
			/*システムプロセスを名前から取得する*/
			utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name) { return game_process_manager_public_interface_.GetSystemProcess(name); }
		private:
			GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}
