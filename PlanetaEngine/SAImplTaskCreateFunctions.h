#pragma once

#include "ScenePublicInterface.h"
#include "TaskManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {
		class SAImplTaskCreateFunctions {
		public:
			explicit SAImplTaskCreateFunctions(core::ScenePublicInterface& spi) :game_process_manager_public_interface_(spi.game_process_manager()) {}
			/**
			* @brief �Q�[���v���Z�X���쐬����
			* @param �ʒu�A�ʒu�w��
			* @return �쐬�����v���Z�X��WeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateTask(const core::TaskPosition& pos) { return game_process_manager_public_interface_.CreateTask<C>(pos); }
			/**
			* @brief �Q�[���v���Z�X���쐬���Ė��O������
			* @param ���O�A�ʒu�A�ʒu�w��
			* @return �쐬�����v���Z�X��WeakPointer
			*/
			template<class C>
			utility::WeakPointer<C> CreateTask(const std::string& name, const core::TaskPosition& pos) { return game_process_manager_public_interface_.CreateTask<C>(name, pos); }
		private:
			TaskManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}