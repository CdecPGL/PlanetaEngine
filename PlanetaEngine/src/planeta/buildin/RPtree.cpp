#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/property_tree/xml_parser.hpp"

#include "planeta/core/FileIStream.hpp"

#include "RPtree.hpp"

namespace plnt {
	//現在はJSONからの読み込みにしか対応していないが、INIやXMLにも対応予定
	bool RPtree::OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) {
		using namespace boost::property_tree;
		std::shared_ptr<ptree> pt = std::make_shared<ptree>();
		FileIStream fis(file);
		try { read_json(fis, *pt); } catch (json_parser_error &e) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。(", e.what(), ")");
			return false;
		}

		ptree_ = std::move(pt);
		return true;
	}

	void RPtree::OnDisposed() { }

	std::shared_ptr<const boost::property_tree::ptree> RPtree::GetPtree() const { return ptree_; }
}
