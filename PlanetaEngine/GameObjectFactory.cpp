#include "GameObjectFactory.h"
#include "SystemLog.h"

namespace planeta_engine {
	GameObjectFactory& GameObjectFactory::GetInstance() {
		static GameObjectFactory gof;
		return gof;
	}

	std::shared_ptr<GameObjectBase> GameObjectFactory::CreateGameObject(const std::string& id) {
		auto it = creator_map_.find(id);
		if (it == creator_map_.end()) {
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないGameObject(ID:", id, "が指定されました。");
			return nullptr;
		} else {
			return (it->second)();
		}
	}

}