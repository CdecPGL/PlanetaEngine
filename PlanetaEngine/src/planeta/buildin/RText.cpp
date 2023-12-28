#include "planeta/core/File.hpp"
#include "..\core\character_code.hpp"

#include "RText.hpp"

namespace plnt {
	bool RText::OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) {
		size_t size = file.size();
		const char *c_ptr = (const char *)file.top_pointer();
		for (unsigned int i = 0; i < size; ++i) { _text.push_back(c_ptr[i]); }
		//UTF8からシステム文字コードへ
		_text = util::convert_utf8_to_system_code(_text);
		return true;
	}

	void RText::OnDisposed() { _text.clear(); }
}
