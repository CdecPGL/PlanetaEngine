#include "TextResource.h"
#include "File.h"
#include "CharacterCode.h"

namespace planeta {
	namespace resources {
		bool TextResource::_Create(const std::shared_ptr<const File>& file)
		{
			if (file->GetStatus() != File::FileStatus::Available) { return false; }
			size_t size = file->GetSize();
			const char* c_ptr = (const char*)file->GetTopPointer();
			for (unsigned int i = 0; i < size; ++i) {
				_text.push_back(c_ptr[i]);
			}
			//UTF8からシステム文字コードへ
			_text = util::ConvertUTF8ToSystemCode(_text);
			return true;
		}

		void TextResource::_Dispose()
		{
			_text.clear();
		}
	}
}


