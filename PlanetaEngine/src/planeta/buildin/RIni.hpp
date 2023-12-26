#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "planeta/core/IniFile.hpp"

namespace plnt {
	class RIni : public ResourceBase {
	public:
		const IniFile &ini_file() const;
		IniFile &ini_file();

	private:
		bool OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) override;
		void OnDisposed() override;
		std::unique_ptr<IniFile> ini_file_;
	};
}
