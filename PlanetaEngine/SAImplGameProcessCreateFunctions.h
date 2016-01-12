#pragma once

#include "ScenePublicInterface.h"
#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {
		class SAImplGameProcessCreateFunctions {
		public:
			explicit SAImplGameProcessCreateFunctions(core::ScenePublicInterface& spi) :game_process_manager_public_interface_(spi.game_process_manager()) {}
			/**
			* @brief ゲームプロセスを作成する
			* @param 位置、位置指定
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const core::GameProcessPosition& pos, InsertPosIndication pos_indication = InsertPosIndication::At) { return game_process_manager_public_interface_.CreateGameProcess<C>(pos, pos_indication); }
			/**
			* @brief ゲームプロセスを作成して名前をつける
			* @param 名前、位置、位置指定
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& name, const core::GameProcessPosition& pos, InsertPosIndication pos_indication = InsertPosIndication::At) { return game_process_manager_public_interface_.CreateGameProcess<C>(name, pos, pos_indication); }
			/**
			* @brief ゲームプロセスを作成する
			* @param キーポジション、位置指定
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& key_pos_id, InsertPosIndication pos_indication = InsertPosIndication::At) {
				try {
					auto pos = game_process_manager_public_interface_.GetKeyPosition(key_pos_id);
					return game_process_manager_public_interface_.CreateGameProcess<C>(pos, pos_indication);
				} catch (std::out_of_range&) {
					return nullptr;
				}
			}
			/**
			* @brief ゲームプロセスを作成して名前をつける
			* @param 名前、キーポジション、位置指定
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& name, const std::string& key_pos_id, InsertPosIndication& pos_indication = InsertPosIndication::At) {
				try {
					auto pos = game_process_manager_public_interface_.GetKeyPosition(key_pos_id);
					return game_process_manager_public_interface_.CreateGameProcess<C>(name, pos, pos_indication);
				} catch (std::out_of_range&) {
					return nullptr;
				}
			}
		private:
			game::GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}