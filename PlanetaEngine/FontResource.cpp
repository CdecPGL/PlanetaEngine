#include <windows.h>
#include "FontResource.h"
#include "File.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace resources {

		bool FontResource::_Create(const std::shared_ptr<file_system::File>& file)
		{
			if (file->GetStatus() != file_system::File::FileStatus::Available) { return false; }
			handle_ = AddFontMemResourceEx(file->GetTopPointer(), file->GetSize(), nullptr, &font_num_);
			if (handle_ == 0) {
				debug::SystemLog::instance().LogError("フォントの読み込みに失敗しました。", "FontResource::_Create");
				return false;
			}
			else {
				return true;
			}			
		}

		void FontResource::_Dispose()
		{
			if (handle_) { RemoveFontMemResourceEx(handle_); }
		}

	}
}
