#pragma once

#include "../core/xml_file.hpp"
#include "../core/resource_base.hpp"

namespace plnt {
	class r_xml final : public resource_base {
	public:
		[[nodiscard]] const xml_file &xml_file() const;
		plnt::xml_file &xml_file();

	private:
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		std::unique_ptr<plnt::xml_file> xml_file_;
	};
}
