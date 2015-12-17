#pragma once

#include <memory>
#include <functional>
#include <string>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "InsertPosIndication.h"
#include "GameProcessPosition.h"

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	}
	namespace game {
		class GameProcess;
		class GameProcessManagerPublicInterface {
		public:
			virtual ~GameProcessManagerPublicInterface() = default;
			/*�Q�[���v���Z�X���쐬����*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const core::GameProcessPosition& pos,InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, pos,pos_indication));
			}
			/*�Q�[���v���Z�X���쐬���Ė��O������*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& name,const core::GameProcessPosition& pos, InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, pos,pos_indication,const std::string& name));
			}
			/*�Q�[���v���Z�X�𖼑O����擾����*/
			virtual utility::WeakPointer<GameProcess> GetProcess(const std::string& name) = 0;
			/*�L�[�|�W�V�������쐬����*/
			virtual bool CreateKeyPosition(const std::string& id,const core::GameProcessPosition& pos,InsertPosIndication ins_ind) = 0;
			/*�L�[�|�W�V�������擾����*/
			virtual core::GameProcessPosition GetKeyPosition(const std::string& id)const = 0;
			/*�L�[�|�W�V�������폜����*/
			virtual bool DeleteKeyPosition(const std::string& id) = 0;
		private:
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPosition& pos, InsertPosIndication pos_indication) = 0;
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPosition& pos, InsertPosIndication pos_indication,const std::string& name) = 0;
		};
	}
}