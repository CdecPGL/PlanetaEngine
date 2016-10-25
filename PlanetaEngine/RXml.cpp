#include "RXml.h"

namespace planeta {
	bool RXml::_Create(const File& file, ResourceReferencer&) {
		xml_file_ = std::make_unique<XmlFile>();
		if (!xml_file_->Load(file)) {
			PE_LOG_ERROR("XMLファイルの読み込みに失敗しました。");
			xml_file_.release();
			return false;
		}
		return true;
	}

	void RXml::_Dispose() {
		xml_file_.release();
	}

	const planeta::XmlFile& RXml::xml_file() const {
		return *xml_file_;
	}

	planeta::XmlFile& RXml::xml_file() {
		return *xml_file_;
	}

}