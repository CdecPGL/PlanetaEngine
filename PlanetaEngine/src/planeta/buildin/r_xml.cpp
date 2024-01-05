#include "r_xml.hpp"

namespace plnt {
	bool r_xml::on_loaded(const file &file, [[maybe_unused]] const json_file &metadata,
	                      [[maybe_unused]] resource_referencer &referencer) {
		xml_file_ = std::make_unique<plnt::xml_file>();
		if (!xml_file_->load(file)) {
			PE_LOG_ERROR("XMLファイルの読み込みに失敗しました。");
			xml_file_.reset();
			return false;
		}
		return true;
	}

	void r_xml::on_disposed() { xml_file_.reset(); }

	const xml_file &r_xml::xml_file() const { return *xml_file_; }

	xml_file &r_xml::xml_file() { return *xml_file_; }
}
