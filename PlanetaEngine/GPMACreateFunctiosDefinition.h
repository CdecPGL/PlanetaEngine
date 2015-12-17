#pragma once

#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace core {
		class GPMACreateFunctionsDefinition {
		public:
			explicit GPMACreateFunctionsDefinition(game::GameProcessManagerPublicInterface& pinterface) :game_process_manager_public_interface_(pinterface) {}
			/**
			* @brief �Q�[���v���Z�X���쐬����
			* @param �ʒu�A�ʒu�w��
			* @return �쐬�����v���Z�X��WeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const core::GameProcessPosition& pos, InsertPosIndication pos_indication = InsertPosIndication::At) { return game_process_manager_public_interface_.CreateGameProcess<C>(pos, pos_indication); }
			/**
			* @brief �Q�[���v���Z�X���쐬���Ė��O������
			* @param ���O�A�ʒu�A�ʒu�w��
			* @return �쐬�����v���Z�X��WeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& name, const core::GameProcessPosition& pos, InsertPosIndication pos_indication = InsertPosIndication::At) { return game_process_manager_public_interface_.CreateGameProcess<C>(name, pos, pos_indication); }
			/**
			* @brief �Q�[���v���Z�X���쐬����
			* @param �L�[�|�W�V�����A�ʒu�w��
			* @return �쐬�����v���Z�X��WeakPointer
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
			* @brief �Q�[���v���Z�X���쐬���Ė��O������
			* @param ���O�A�L�[�|�W�V�����A�ʒu�w��
			* @return �쐬�����v���Z�X��WeakPointer
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