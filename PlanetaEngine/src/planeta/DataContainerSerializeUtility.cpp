#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "DataContainerSerializeUtility.hpp"
#include "DataContainerFunctionsForBoostSerialize.hpp"
#include "FileOStream.hpp"
#include "planeta/FileIStream.hpp"
#include "planeta/File.hpp"

namespace plnt {
	namespace util {

		std::shared_ptr<File> SerializeDataContainer(DataContainer& data_container)
		{
			std::shared_ptr<File> file = std::make_shared<File>();
			FileOStream os(*file);
			boost::archive::xml_oarchive xoa(os);
			xoa << boost::serialization::make_nvp("data_container",data_container);
			return std::move(file);
		}

		DataContainer DeserializeDataContainer(const std::shared_ptr<const File>& file)
		{
			FileIStream is(const_cast<File&>(*file));
			boost::archive::xml_iarchive xia(is);
			DataContainer game_data;
			xia >> boost::serialization::make_nvp("data_container", game_data);
			return std::move(game_data);
		}

	}
}