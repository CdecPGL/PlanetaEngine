#pragma once

#include "XmlFile.h"
#include "ResourceBase.h"

namespace planeta {
	class RXml final : public private_::ResourceBase {
	public:
		const XmlFile& xml_file()const;
		XmlFile& xml_file();
	private:
		bool _Create(const File& file) override;
		void _Dispose() override;
		std::unique_ptr<XmlFile> xml_file_;
	};
}