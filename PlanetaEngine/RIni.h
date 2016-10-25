#pragma once

#include "ResourceBase.h"
#include "IniFile.h"

namespace planeta {
	class RIni : public ResourceBase {
	public:
		const IniFile& ini_file()const;
		IniFile& ini_file();
	private:
		bool _Create(const File& file, ResourceReferencer&) override;
		void _Dispose()override;
		std::unique_ptr<IniFile> ini_file_;
	};
}
