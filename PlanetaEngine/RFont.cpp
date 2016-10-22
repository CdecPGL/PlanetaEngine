#include <windows.h>
#include "RFont.h"
#include "File.h"
#include "SystemLog.h"

namespace planeta {
	bool RFont::_Create(const File& file, private_::ResourceReferencer&) {
		handle_ = AddFontMemResourceEx(const_cast<unsigned char*>(file.top_pointer()), file.size(), nullptr, &font_num_);
		if (handle_ == 0) {
			PE_LOG_ERROR("フォントの読み込みに失敗しました。");
			return false;
		} else {
			return true;
		}
	}

	void RFont::_Dispose() {
		if (handle_) { RemoveFontMemResourceEx(handle_); }
	}
}
