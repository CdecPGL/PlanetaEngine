#include "TextResource.h"
#include "File.h"

namespace planeta_engine {
	namespace resources {
		bool TextResource::_Create(const std::shared_ptr<const file_system::File>& file)
		{
			if (file->GetStatus() != file_system::File::FileStatus::Available) { return false; }
			size_t size = file->GetSize();
			const char* c_ptr = (const char*)file->GetTopPointer();
			for (unsigned int i = 0; i < file->GetSize(); ++i) {
				_text.push_back(c_ptr[i]);
			}
			return true;
		}

		void TextResource::_Dispose()
		{
			_text.clear();
		}
	}
}


