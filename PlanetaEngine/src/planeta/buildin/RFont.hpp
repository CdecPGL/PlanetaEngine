#pragma once

#include <windows.h>

#include "planeta/core/ResourceBase.hpp"

namespace plnt {
	/*フォントリソース*/
	class RFont final : public resource_base {
	public:
		RFont() { };

		~RFont() { };
		HANDLE GetHandle() const { return handle_; }
		int GetDXHandle() const { return dx_handle_; }

	private:
		HANDLE handle_ = 0;
		DWORD font_num_ = 0;
		int dx_handle_;
		int size_ = 0;
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
	};
}
