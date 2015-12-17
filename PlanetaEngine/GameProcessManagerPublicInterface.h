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
			/*ゲームプロセスを作成する*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const core::GameProcessPosition& pos,InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, pos,pos_indication));
			}
			/*ゲームプロセスを作成して名前をつける*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& name,const core::GameProcessPosition& pos, InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, pos,pos_indication,const std::string& name));
			}
			/*ゲームプロセスを名前から取得する*/
			virtual utility::WeakPointer<GameProcess> GetProcess(const std::string& name) = 0;
			/*キーポジションを作成する*/
			virtual bool CreateKeyPosition(const std::string& id,const core::GameProcessPosition& pos,InsertPosIndication ins_ind) = 0;
			/*キーポジションを取得する*/
			virtual core::GameProcessPosition GetKeyPosition(const std::string& id)const = 0;
			/*キーポジションを削除する*/
			virtual bool DeleteKeyPosition(const std::string& id) = 0;
		private:
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPosition& pos, InsertPosIndication pos_indication) = 0;
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPosition& pos, InsertPosIndication pos_indication,const std::string& name) = 0;
		};
	}
}