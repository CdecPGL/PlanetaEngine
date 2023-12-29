#pragma once

#include "../core/xml_file.hpp"
#include "planeta/core/ResourceBase.hpp"

namespace plnt {
	class RXml final : public ResourceBase {
	public:
		[[nodiscard]] const xml_file &xml_file() const;
		plnt::xml_file &xml_file();

	private:
		bool OnLoaded(const file &file, const json_file &metadata, ResourceReferencer &referencer) override;
		void OnDisposed() override;
		std::unique_ptr<plnt::xml_file> xml_file_;
	};
}
