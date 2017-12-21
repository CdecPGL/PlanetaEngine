#include <windows.h>
#include "RFont.h"
#include "File.h"
#include "LogUtility.h"

namespace planeta {
	bool RFont::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
		handle_ = AddFontMemResourceEx(const_cast<unsigned char*>(file.top_pointer()), file.size(), nullptr, &font_num_);
		if (handle_ == 0) {
			PE_LOG_ERROR("フォントの読み込みに失敗しました。");
			return false;
		} else {
			return true;
		}
	}

	void RFont::OnDisposed() {
		if (handle_) { RemoveFontMemResourceEx(handle_); }
	}
}
