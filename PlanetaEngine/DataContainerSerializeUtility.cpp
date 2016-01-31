#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "DataContainerSerializeUtility.h"
#include "DataContainerFunctionsForBoostSerialize.h"
#include "FileOStream.h"
#include "FileIStream.h"
#include "File.h"

namespace planeta_engine {
	namespace utility {

		std::shared_ptr<file_system::File> SerializeDataContainer(DataContainer& data_container)
		{
			std::shared_ptr<file_system::File> file = std::make_shared<file_system::File>();
			file_system::FileOStream os(*file);
			boost::archive::xml_oarchive xoa(os);
			xoa << boost::serialization::make_nvp("data_container",data_container);
			return std::move(file);
		}

		DataContainer DeserializeDataContainer(const std::shared_ptr<const file_system::File>& file)
		{
			file_system::FileIStream is(const_cast<file_system::File&>(*file));
			boost::archive::xml_iarchive xia(is);
			DataContainer game_data;
			xia >> boost::serialization::make_nvp("data_container", game_data);
			return std::move(game_data);
		}

	}
}