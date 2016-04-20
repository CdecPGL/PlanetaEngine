#pragma once

#include <memory>
#include <functional>
#include <string>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "InsertPosIndication.h"
#include "TaskPosition.h"

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	}
	namespace game {
		class Task;
		class TaskManagerPublicInterface {
		public:
			virtual ~TaskManagerPublicInterface() = default;
			/*�Q�[���v���Z�X���쐬����*/
			template<class C>
			utility::WeakPointer<C> CreateTask(const core::TaskPosition& pos) {
				static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
				return std::static_pointer_cast<C>(CreateTask([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, pos));
			}
			/*�Q�[���v���Z�X���쐬���Ė��O������*/
			template<class C>
			utility::WeakPointer<C> CreateTask(const std::string& name,const core::TaskPosition& pos) {
				static_assert(std::is_base_of<Task, C>::value == true, "C is not derived Task.");
				return std::static_pointer_cast<C>(CreateTask([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, pos,const std::string& name));
			}
			/*�Q�[���v���Z�X�𖼑O����擾����*/
			virtual utility::WeakPointer<Task> GetProcess(const std::string& name) = 0;
			/*�L�[�|�W�V�������擾����*/
			virtual core::TaskPosition GetKeyPosition(const std::string& id)const = 0;
		private:
			virtual std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, const core::TaskPosition& pos) = 0;
			virtual std::shared_ptr<Task> CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, const core::TaskPosition& pos, const std::string& name) = 0;
		};
	}
}