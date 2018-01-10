#include "planeta/RFont.hpp"
#include "File.hpp"
#include "LogUtility.hpp"
#include "JsonFile.hpp"
#include "DxLib.h"

namespace plnt {
	bool RFont::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
		handle_ = AddFontMemResourceEx(const_cast<unsigned char*>(file.top_pointer()), file.size(), nullptr, &font_num_);
		if (handle_ == 0) {
			PE_LOG_ERROR("フォントの読み込みに失敗しました。");
			return false;
		}
		auto root = metadata.GetRoot();
		auto root_obj = root.Get<JSONObject>();
		auto name = *root_obj->AtWithException("name")->GetWithException<std::string>();
		auto size = *root_obj->AtWithException("size")->GetWithException<int>();
		auto thick = *root_obj->AtWithException("thick")->GetWithException<int>();
		auto outline = *root_obj->AtWithException("outline")->GetWithException<bool>();
		auto antialiasing = *root_obj->AtWithException("antialiasing")->GetWithException<bool>();
		//DXフォントタイプを求める
		int dx_font_type = outline ? (antialiasing ? DX_FONTTYPE_ANTIALIASING_EDGE : DX_FONTTYPE_EDGE) : (antialiasing ? DX_FONTTYPE_ANTIALIASING : DX_FONTTYPE_NORMAL);
		//DXライブラリフォントハンドルを作成
		dx_handle_ = CreateFontToHandle(name.c_str(), size, thick, dx_font_type);
		if (handle_ < 0) {
			std::ostringstream err_str_str;
			err_str_str << "フォントハンドルの作成に失敗しました。(";
			err_str_str << "name=" << name << ",size=" << size << ",thick=" << thick;
			err_str_str << ",outline=" << (outline ? "true" : "false") << ",antialiasing=" << (antialiasing ? "true" : "false") << ")";
			PE_LOG_ERROR(err_str_str.str());
			return false;
		}
		size_ = GetFontSizeToHandle(dx_handle_);
		return true;
	}

	void RFont::OnDisposed() {
		if (handle_ >= 0) { DeleteFontToHandle(dx_handle_); }
		if (handle_) { RemoveFontMemResourceEx(handle_); }
	}
}
