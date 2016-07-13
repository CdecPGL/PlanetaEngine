#include <windows.h>
#include "RFont.h"
#include "File.h"
#include "SystemLog.h"

namespace planeta {
	bool RFont::_Create(const std::shared_ptr<const File>& file) {
		if (file->GetStatus() != File::FileStatus::Available) { return false; }
		handle_ = AddFontMemResourceEx(const_cast<unsigned char*>(file->GetTopPointer()), file->GetSize(), nullptr, &font_num_);
		if (handle_ == 0) {
			PE_LOG_ERROR("�t�H���g�̓ǂݍ��݂Ɏ��s���܂����B");
			return false;
		} else {
			return true;
		}
	}

	void RFont::_Dispose() {
		if (handle_) { RemoveFontMemResourceEx(handle_); }
	}
}
