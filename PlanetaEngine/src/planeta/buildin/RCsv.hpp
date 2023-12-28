#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "../core/csv_file.hpp"

namespace plnt {
	class RCsv final : public ResourceBase {
	public:
		[[nodiscard]] const csv_file &csv_file() const;
		plnt::csv_file &csv_file();

	private:
		std::unique_ptr<plnt::csv_file> csv_file_;

		bool OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) override;
		void OnDisposed() override;
	};
}
