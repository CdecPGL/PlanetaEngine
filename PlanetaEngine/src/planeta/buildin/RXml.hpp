#pragma once

#include "planeta/core/XmlFile.hpp"
#include "planeta/ResourceBase.hpp"

namespace plnt {
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