#include "DxLib.h"

#include "planeta/core/file.hpp"
#include "../core/log_utility.hpp"
#include "../core/json_file.hpp"
#include "r_font.hpp"

namespace plnt {
	bool r_font::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		handle_ = AddFontMemResourceEx(const_cast<unsigned char *>(file.top_pointer()), file.size(), nullptr,
		                               &font_num_);
		if (handle_ == nullptr) {
			PE_LOG_ERROR("フォントの読み込みに失敗しました。");
			return false;
		}
		const auto root = metadata.get_root();
		const auto root_obj = root.get<json_object>();
		const auto name = *root_obj->at_with_exception("name")->get_with_exception<std::string>();
		const auto size = *root_obj->at_with_exception("size")->get_with_exception<int>();
		const auto thick = *root_obj->at_with_exception("thick")->get_with_exception<int>();
		const auto outline = *root_obj->at_with_exception("outline")->get_with_exception<bool>();
		const auto antialiasing = *root_obj->at_with_exception("antialiasing")->get_with_exception<bool>();
		//DXフォントタイプを求める
		const int dx_font_type = outline
			                         ? (antialiasing ? DX_FONTTYPE_ANTIALIASING_EDGE : DX_FONTTYPE_EDGE)
			                         : (antialiasing ? DX_FONTTYPE_ANTIALIASING : DX_FONTTYPE_NORMAL);
		//DXライブラリフォントハンドルを作成
		dx_handle_ = CreateFontToHandle(name.c_str(), size, thick, dx_font_type);
		if (handle_ == nullptr) {
			std::ostringstream err_str_str;
			err_str_str << "フォントハンドルの作成に失敗しました。(";
			err_str_str << "name=" << name << ",size=" << size << ",thick=" << thick;
			err_str_str << ",outline=" << (outline ? "true" : "false") << ",antialiasing=" << (
				antialiasing ? "true" : "false") << ")";
			PE_LOG_ERROR(err_str_str.str());
			return false;
		}
		size_ = GetFontSizeToHandle(dx_handle_);
		return true;
	}

	void r_font::on_disposed() {
		if (handle_ != nullptr) { DeleteFontToHandle(dx_handle_); }
		if (handle_ != nullptr) { RemoveFontMemResourceEx(handle_); }
	}
}
