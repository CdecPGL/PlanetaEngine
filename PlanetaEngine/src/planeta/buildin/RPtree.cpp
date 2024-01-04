#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/property_tree/xml_parser.hpp"

#include "../core/file_istream.hpp"

#include "RPtree.hpp"

namespace plnt {
	//現在はJSONからの読み込みにしか対応していないが、INIやXMLにも対応予定
	bool r_ptree::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		using namespace boost::property_tree;
		auto pt = std::make_shared<ptree>();
		file_istream fis(file);
		try { read_json(fis, *pt); } catch (json_parser_error &e) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。(", e.what(), ")");
			return false;
		}

		ptree_ = std::move(pt);
		return true;
	}

	void r_ptree::on_disposed() { }

	std::shared_ptr<const boost::property_tree::ptree> r_ptree::get_ptree() const { return ptree_; }
}
