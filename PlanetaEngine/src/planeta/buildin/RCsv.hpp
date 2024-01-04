#pragma once

#include "../core/resource_base.hpp"
#include "../core/csv_file.hpp"

namespace plnt {
	class r_csv final : public resource_base {
	public:
		[[nodiscard]] const csv_file &csv_file() const;
		plnt::csv_file &csv_file();

	private:
		std::unique_ptr<plnt::csv_file> csv_file_;

		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
	};
}
