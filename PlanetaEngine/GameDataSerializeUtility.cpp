#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "GameDataSerializeUtility.h"
#include "GameDataFunctionsForBoostSerialize.h"
#include "FileOStream.h"
#include "FileIStream.h"
#include "File.h"

namespace planeta_engine {
	namespace core {

		std::shared_ptr<file_system::File> SerializeGameDataElement(GameDataElement& game_data_element)
		{
			std::shared_ptr<file_system::File> file = std::make_shared<file_system::File>();
			file_system::FileOStream os(*file);
			boost::archive::xml_oarchive xoa(os);
			xoa << boost::serialization::make_nvp("game_data_element",game_data_element);
			return file;
		}

		planeta_engine::core::GameDataElement DeserializeGameDataElement(const std::shared_ptr<file_system::File>& file)
		{
			file_system::FileIStream is(*file);
			boost::archive::xml_iarchive xia(is);
			GameDataElement game_data_element = GameDataElement::CreateByTypeID(GameDataElementType::ConvertTypeToTypeID(GameDataElementType::Type::nil));
			xia >> boost::serialization::make_nvp("game_data_element", game_data_element);
			return std::move(game_data_element);
		}

	}
}