#include "planeta/core/file.hpp"
#include "../core/character_code.hpp"

#include "r_text.hpp"

namespace plnt {
	bool r_text::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		const size_t size = file.size();
		const auto c_ptr = reinterpret_cast<const char *>(file.top_pointer());
		for (unsigned int i = 0; i < size; ++i) { text_.push_back(c_ptr[i]); }
		//UTF8からシステム文字コードへ
		text_ = util::convert_utf8_to_system_code(text_);
		return true;
	}

	void r_text::on_disposed() { text_.clear(); }
}
