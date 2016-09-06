#include "RPtree.h"

#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/property_tree/xml_parser.hpp"

#include "FileIStream.h"

namespace planeta {
	bool RPtree::_Create(const std::shared_ptr<const File>& file) {
		using namespace boost::property_tree;
		std::shared_ptr<ptree> pt = std::make_shared<ptree>();
		FileIStream fis(*file);
		read_json(fis, *pt);

		ptree_ = std::move(pt);
		return true;
	}

	void RPtree::_Dispose() {
		
	}

	std::shared_ptr<const boost::property_tree::ptree> RPtree::GetPtree() const {
		return ptree_;
	}

}