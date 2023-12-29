#include "DxLib.h"

#include "planeta/core/file.hpp"
#include "planeta/core/LogUtility.hpp"
#include "..\core\json_file.hpp"

#include "RFont.hpp"

namespace plnt {
	bool RFont::OnLoaded(const file &file, const json_file &metadata, ResourceReferencer &referencer) {
		handle_ = AddFontMemResourceEx(const_cast<unsigned char *>(file.top_pointer()), file.size(), nullptr,
		                               &font_num_);
		if (handle_ == 0) {
			PE_LOG_ERROR("フォントの読み込みに失敗しました。");
			return false;
		}
		auto root = metadata.get_root();
		auto root_obj = root.get<json_object>();
		auto name = *root_obj->at_with_exception("name")->get_with_exception<std::string>();
		auto size = *root_obj->at_with_exception("size")->get_with_exception<int>();
		auto thick = *root_obj->at_with_exception("thick")->get_with_exception<int>();
		auto outline = *root_obj->at_with_exception("outline")->get_with_exception<bool>();
		auto antialiasing = *root_obj->at_with_exception("antialiasing")->get_with_exception<bool>();
		//DXフォントタイプを求める
		int dx_font_type = outline
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

	void RFont::OnDisposed() {
		if (handle_ != nullptr) { DeleteFontToHandle(dx_handle_); }
		if (handle_ != nullptr) { RemoveFontMemResourceEx(handle_); }
	}
}
