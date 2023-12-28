#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"

#include "data_container_serialize_utility.hpp"
#include "data_container_functions_for_boost_serialize.hpp"
#include "file_ostream.hpp"
#include "file_istream.hpp"
#include "file.hpp"

namespace plnt::util {
	std::shared_ptr<file> serialize_data_container(data_container &data_container) {
		auto f = std::make_shared<file>();
		file_ostream os(*f);
		boost::archive::xml_oarchive xoa(os);
		xoa << boost::serialization::make_nvp("data_container", data_container);
		return f;
	}

	data_container deserialize_data_container(const std::shared_ptr<const file> &f) {
		file_istream is(const_cast<file &>(*f));
		boost::archive::xml_iarchive xia(is);
		data_container game_data;
		xia >> boost::serialization::make_nvp("data_container", game_data);
		return game_data;
	}
}
