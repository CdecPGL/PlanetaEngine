#include "GameDataManager.h"

#include "GameDataSerializeUtility.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace core {

		std::shared_ptr<GameDataElement> GameDataManager::GetElement(const std::string& data_id)
		{
			auto it = datas_.find(data_id);
			return it == datas_.end() ? nullptr : it->second;
		}

		std::shared_ptr<GameDataElement> GameDataManager::SetElement(const std::string& data_id, GameDataElement&& data)
		{
			return datas_.emplace(data_id, std::make_shared<GameDataElement>(std::move(data))).first->second; 
		}

		bool GameDataManager::SaveGameData(const std::string& data_id)
		{
			auto it = datas_.find(data_id);
			if (it == datas_.end()) { 
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "‘¶Ý‚µ‚È‚¢GameData", data_id, "‚ð•Û‘¶‚µ‚æ‚¤‚Æ‚µ‚Ü‚µ‚½B");
				return false; 
			}
			auto file = SerializeGameDataElement(*it->second);

			return true;
		}

	}
}