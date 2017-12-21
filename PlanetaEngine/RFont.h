#pragma once

#include <windef.h>
#include "ResourceBase.h"
#include "Vector2D.h"

namespace planeta {
	/*フォントリソース*/
	class RFont final : public ResourceBase {
	public:
		RFont() {};
		~RFont() {};
		HANDLE GetHandle()const { return handle_; }
	private:
		HANDLE handle_ = 0;
		DWORD font_num_ = 0;
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
	};
}