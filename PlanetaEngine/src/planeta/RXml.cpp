#include "planeta/RXml.hpp"

namespace plnt {
	bool RXml::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
		xml_file_ = std::make_unique<XmlFile>();
		if (!xml_file_->Load(file)) {
			PE_LOG_ERROR("XMLファイルの読み込みに失敗しました。");
			xml_file_.release();
			return false;
		}
		return true;
	}

	void RXml::OnDisposed() {
		xml_file_.release();
	}

	const plnt::XmlFile& RXml::xml_file() const {
		return *xml_file_;
	}

	plnt::XmlFile& RXml::xml_file() {
		return *xml_file_;
	}

}