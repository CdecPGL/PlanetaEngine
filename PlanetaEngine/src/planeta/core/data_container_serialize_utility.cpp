#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"

#include "data_container_serialize_utility.hpp"
#include "data_container_functions_for_boost_serialize.hpp"
#include "FileOStream.hpp"
#include "FileIStream.hpp"
#include "File.hpp"

namespace plnt::util {
	std::shared_ptr<File> serialize_data_container(data_container &data_container) {
		auto file = std::make_shared<File>();
		FileOStream os(*file);
		boost::archive::xml_oarchive xoa(os);
		xoa << boost::serialization::make_nvp("data_container", data_container);
		return file;
	}

	data_container deserialize_data_container(const std::shared_ptr<const File> &file) {
		FileIStream is(const_cast<File &>(*file));
		boost::archive::xml_iarchive xia(is);
		data_container game_data;
		xia >> boost::serialization::make_nvp("data_container", game_data);
		return game_data;
	}
}
