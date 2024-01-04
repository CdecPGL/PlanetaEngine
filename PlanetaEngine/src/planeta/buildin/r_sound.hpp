#pragma once

#include "../core/resource_base.hpp"

namespace plnt {
	/*効果音リソース*/
	class r_sound final : public resource_base {
	public:
		r_sound() : handle_(-1) { }

		[[nodiscard]] int get_handle() const { return handle_; }
		[[nodiscard]] int get_total_time_by_milli_second() const;

	private:
		int handle_;
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
	};
}
