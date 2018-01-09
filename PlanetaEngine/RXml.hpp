#pragma once

#include "XmlFile.h"
#include "ResourceBase.h"

namespace planeta {
	class RXml final : public ResourceBase {
	public:
		const XmlFile& xml_file()const;
		XmlFile& xml_file();
	private:
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
		std::unique_ptr<XmlFile> xml_file_;
	};
}