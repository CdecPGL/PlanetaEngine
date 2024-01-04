#pragma once

#include <windows.h>

#include "../core/resource_base.hpp"

namespace plnt {
	/*フォントリソース*/
	class r_font final : public resource_base {
	public:
		[[nodiscard]] HANDLE get_handle() const { return handle_; }
		[[nodiscard]] int get_dx_handle() const { return dx_handle_; }

	private:
		HANDLE handle_ = nullptr;
		DWORD font_num_ = 0;
		int dx_handle_ = 0;
		int size_ = 0;
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
	};
}
