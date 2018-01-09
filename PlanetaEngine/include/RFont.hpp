#pragma once

#include <windows.h>
#include "ResourceBase.hpp"

namespace plnt {
	/*フォントリソース*/
	class RFont final : public ResourceBase {
	public:
		RFont() {};
		~RFont() {};
		HANDLE GetHandle()const { return handle_; }
		int GetDXHandle()const { return dx_handle_; }
	private:
		HANDLE handle_ = 0;
		DWORD font_num_ = 0;
		int dx_handle_;
		int size_ = 0;
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
	};
}