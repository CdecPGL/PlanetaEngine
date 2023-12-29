#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "../core/ini_file.hpp"

namespace plnt {
	class RIni : public ResourceBase {
	public:
		const ini_file &get_ini_file() const;
		ini_file &get_ini_file();

	private:
		bool OnLoaded(const file &file, const json_file &metadata, ResourceReferencer &referencer) override;
		void OnDisposed() override;
		std::unique_ptr<ini_file> ini_file_;
	};
}
