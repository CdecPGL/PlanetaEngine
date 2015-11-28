#include "GameDataManager.h"

#include "GameDataSerializeUtility.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "FileSystemManager.h"
#include "SystemVariables.h"
#include "GameDataElementType.h"

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
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���݂��Ȃ�GameData(", data_id, ")��ۑ����悤�Ƃ��܂����B");
				return false; 
			}
			auto file = SerializeGameDataElement(*it->second);
			if (file_accessor_->SaveFile(data_id, std::move(*file))) {
				debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "GameData(", data_id, ")��ۑ����܂����B");
				return true;
			}
			else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "GameData(", data_id, ")�̕ۑ��Ɏ��s���܂����B");
				return false;
			}		
		}

		bool GameDataManager::LoadGameData(const std::string& file_name)
		{
			try {
				auto file = file_accessor_->LoadFile(file_name);
				if (!file) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�t�@�C��(", file_name, ")�̓ǂݍ��݂Ɏ��s���܂����B");
					return false;
				}
				auto game_element = std::move(DeserializeGameDataElement(file));
				datas_.emplace(file_name, std::make_shared<GameDataElement>(std::move(game_element)));
				debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "GameData(", file_name, ")��ǂݍ��݂܂����B");
				return true;
			}
			catch (std::runtime_error&) {
				debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "GameData(", file_name, ")�̓ǂݍ��݂Ɏ��s���܂����B");
				return false;
			}
		}

		bool GameDataManager::Initialize()
		{
			if (!CheckComplexTypeElementTypeError()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCDNAME__, "�������Ɏ��s���܂����B�o�^���ꂽGameData�����^�̌^�`�F�b�N�Ɏ��s���܂����B");
				return false;
			}
			file_accessor_ = file_system::FileSystemManager::instance().GetFileAccessor(system_variables::GameDataFileAccessorID);
			if (!file_accessor_) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCDNAME__, "�������Ɏ��s���܂����B�t�@�C���A�N�Z�T�̎擾�Ɏ��s���܂����B");
				return false;
			}
			else {
				return true;
			}
		}

		void GameDataManager::ResistComplexType(const std::string& type_id, std::unordered_map<std::string, std::string>&& element_types)
		{
			GameDataElementType::ResistComplexType(type_id, std::make_unique<GameDataComplexType>(type_id, std::move(element_types)));
		}

		bool GameDataManager::CheckComplexTypeElementTypeError()
		{
			return GameDataElementType::ComplexTypeElementTypeCheck();
		}

		bool GameDataManager::Finalize()
		{
			file_accessor_.reset();
			return true;
		}

	}
}