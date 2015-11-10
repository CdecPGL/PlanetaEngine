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
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "存在しないGameData(", data_id, ")を保存しようとしました。");
				return false; 
			}
			auto file = SerializeGameDataElement(*it->second);
			debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "GameData(", data_id, ")を保存しました。");
			return true;
		}

		bool GameDataManager::LoadGameData(const std::string& file_name)
		{
			debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "GameData(", file_name, ")を読み込みました。");
			debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "GameData(", file_name, ")の読み込みに失敗しました。");
			return false;
		}

	}
}