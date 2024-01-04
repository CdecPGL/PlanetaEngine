#pragma once

#include "../core/resource_base.hpp"
#include "../core/ini_file.hpp"

namespace plnt {
	class r_ini final : public resource_base {
	public:
		[[nodiscard]] const ini_file &get_ini_file() const;
		ini_file &get_ini_file();

	private:
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		std::unique_ptr<ini_file> ini_file_;
	};
}
