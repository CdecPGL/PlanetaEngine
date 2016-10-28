#include "StandardDebugManager.h"
#include "LogUtility.h"
#include "DxLib.h"

namespace planeta {
	namespace private_ {

		//////////////////////////////////////////////////////////////////////////
		//Impl
		//////////////////////////////////////////////////////////////////////////

		class StandardDebugManager::Impl_ {
		public:
			
		};

		//////////////////////////////////////////////////////////////////////////
		//StandardDebugManager
		//////////////////////////////////////////////////////////////////////////

		StandardDebugManager::StandardDebugManager() :impl_(std::make_unique<Impl_>()) {}
		StandardDebugManager::~StandardDebugManager() = default;

		bool StandardDebugManager::Initialize() {

			return true;
		}

		void StandardDebugManager::Finalize() {
			
		}

		void StandardDebugManager::Update() {
			
		}

		bool StandardDebugManager::CreateDebugInformationChannel(const std::string& channel_id, const std::function<void(IDebugInformationAdder &) >& handler) {
			throw std::logic_error("The method or operation is not implemented.");
		}

		bool StandardDebugManager::DeleteDebugInformationChannel(const std::string& channel_id) {
			throw std::logic_error("The method or operation is not implemented.");
		}

		bool StandardDebugManager::CreateDebugDrawChannel(const std::string& channel_id, const std::function<void(IDebugDrawer &) > handler) {
			throw std::logic_error("The method or operation is not implemented.");
		}

		bool StandardDebugManager::DeleteDebugDrawChannel(const std::string& channel_id) {
			throw std::logic_error("The method or operation is not implemented.");
		}

	}
}