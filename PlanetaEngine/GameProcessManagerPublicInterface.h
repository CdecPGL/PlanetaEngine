#pragma once

#include <memory>
#include <functional>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "InsertPosIndication.h"

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
		class GameProcessPositionInList;
	}
	namespace game {
		class GameProcess;
		class GameProcessManagerPublicInterface {
		public:
			virtual ~GameProcessManagerPublicInterface() = default;
			/*�Q�[���v���Z�X���쐬����*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const core::GameProcessPositionInList& pos,InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, pos,pos_indication));
			}
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& key_pos_id, InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, key_pos_id,pos_indication));
			}
			/*�Q�[���v���Z�X���쐬���Ė��O������*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& name,const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, pos,pos_indication,const std::string& name));
			}
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& name, const std::string& key_pos_id, InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, key_pos_id, pos_indication, const std::string& name));
			}
			/*�Q�[���v���Z�X�𖼑O����擾����*/
			virtual utility::WeakPointer<GameProcess> GetProcess(const std::string& name) = 0;
			/*�L�[�|�W�V�������쐬����*/
			virtual bool CreateKeyPosition(const std::string& id,const core::GameProcessPositionInList& pos,InsertPosIndication ins_ind) = 0;
			/*�L�[�|�W�V�������폜����*/
			virtual bool DeleteKeyPosition(const std::string& id) = 0;
		private:
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication) = 0;
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication,const std::string& name) = 0;
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const std::string& key_pos_id, InsertPosIndication pos_indication) = 0;
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const std::string& key_pos_id, InsertPosIndication pos_indication, const std::string& name) = 0;
		};
	}
}