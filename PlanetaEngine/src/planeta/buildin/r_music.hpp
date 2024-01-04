#pragma once

#include "../core/resource_base.hpp"

namespace plnt {
	/*音楽リソース*/
	class r_music final : public resource_base {
	public:
		r_music() : handle_(-1) { }

		[[nodiscard]] int get_handle() const { return handle_; }
		[[nodiscard]] int get_total_time_by_milli_second() const;

	private:
		int handle_;
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
	};
}
