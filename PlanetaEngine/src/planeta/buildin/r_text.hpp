#pragma once

#include "../core/resource_base.hpp"

namespace plnt {
	class r_text final : public resource_base {
	public:
		[[nodiscard]] const std::string &get_text() const { return text_; }

	private:
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		std::string text_;
	};
}
