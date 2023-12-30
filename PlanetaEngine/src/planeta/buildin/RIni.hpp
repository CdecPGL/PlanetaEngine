#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "../core/ini_file.hpp"

namespace plnt {
	class RIni : public resource_base {
	public:
		const ini_file &get_ini_file() const;
		ini_file &get_ini_file();

	private:
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		std::unique_ptr<ini_file> ini_file_;
	};
}
