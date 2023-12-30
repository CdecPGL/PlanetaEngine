#pragma once

#include "../core/xml_file.hpp"
#include "planeta/core/ResourceBase.hpp"

namespace plnt {
	class RXml final : public resource_base {
	public:
		[[nodiscard]] const xml_file &xml_file() const;
		plnt::xml_file &xml_file();

	private:
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		std::unique_ptr<plnt::xml_file> xml_file_;
	};
}
