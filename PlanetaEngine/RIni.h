#pragma once

#include "ResourceBase.h"
#include "IniFile.h"

namespace planeta {
	class RIni : public private_::ResourceBase {
	public:
		const IniFile& ini_file()const;
		IniFile& ini_file();
	private:
		bool _Create(const File& file, private_::ResourceReferencer&) override;
		void _Dispose()override;
		std::unique_ptr<IniFile> ini_file_;
	};
}
