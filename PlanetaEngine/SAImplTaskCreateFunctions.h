#pragma once

#include "ScenePublicInterface.h"
#include "TaskManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {
		class SAImplTaskCreateFunctions {
		public:
			explicit SAImplTaskCreateFunctions(core::ScenePublicInterface& spi) :game_process_manager_public_interface_(spi.game_process_manager()) {}
			/**
			* @brief ゲームプロセスを作成する
			* @param 位置、位置指定
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateTask(const core::TaskPosition& pos) { return game_process_manager_public_interface_.CreateTask<C>(pos); }
			/**
			* @brief ゲームプロセスを作成して名前をつける
			* @param 名前、位置、位置指定
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateTask(const std::string& name, const core::TaskPosition& pos) { return game_process_manager_public_interface_.CreateTask<C>(name, pos); }
			/**
			* @brief ゲームプロセスを作成する
			* @param キーポジション、位置指定
			* @return 作成したプロセスのWeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateTask(const std::string& key_pos_id) {
				try {
					auto pos = game_process_manager_public_interface_.GetKeyPosition(key_pos_id);
					return game_process_manager_public_interface_.CreateTask<C>(pos);
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
			utility::WeakPointer<C> CreateTask(const std::string& name, const std::string& key_pos_id) {
				try {
					auto pos = game_process_manager_public_interface_.GetKeyPosition(key_pos_id);
					return game_process_manager_public_interface_.CreateTask<C>(name, pos);
				} catch (std::out_of_range&) {
					return nullptr;
				}
			}
		private:
			game::TaskManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}