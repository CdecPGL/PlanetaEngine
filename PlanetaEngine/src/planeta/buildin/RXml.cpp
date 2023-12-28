#include "RXml.hpp"

namespace plnt {
	bool RXml::OnLoaded(const file &file, const JsonFile &metadata, ResourceReferencer &referencer) {
		xml_file_ = std::make_unique<plnt::xml_file>();
		if (!xml_file_->load(file)) {
			PE_LOG_ERROR("XMLファイルの読み込みに失敗しました。");
			xml_file_.reset();
			return false;
		}
		return true;
	}

	void RXml::OnDisposed() { xml_file_.reset(); }

	const xml_file &RXml::xml_file() const { return *xml_file_; }

	xml_file &RXml::xml_file() { return *xml_file_; }
}
