#include "RFontDefinition.h"

#include <windows.h>
#include <sstream>
#include "boost/lexical_cast.hpp"
#include "DxLib.h"
#include "XmlFile.h"
#include "File.h"
#include "FileSystemManager.h"
#include "SystemLog.h"
#include "SystemVariables.h"
#include "FileSystemManager.h"
#include "FileAccessor.h"
#include "StandardResourceManager.h"
#include "RFont.h"

namespace planeta {
	bool RFontDefinition::_Create(const File& file, private_::ResourceReferencer& referencer) {
		auto file_accessor = FileSystemManager::instance().GetFileAccessor(private_::system_variables::file_system::ResourceFileAccessorID);
		XmlFile xml{};
		if (xml.Load(file)) {
			auto root = xml.GetRootElement();
			auto fd_elem = root->FindFirstChild("font_definition");
			std::string resource_id_or_path = "<リソースID又はパスが未指定>";
			if (fd_elem) {
				auto elem = fd_elem->FindFirstChild("font_resource_id_or_path");
				//フォントファイルが指定されていたら読み込み
				if (elem) {
					resource_id_or_path = elem->text();
					//指定されたフォントリソース取得
					auto f_res = referencer.ReferenceResourceByIDorPath<RFont>(resource_id_or_path);
					if (f_res == nullptr) {
						PE_LOG_ERROR("フォントリソースの取得に失敗しました。(", elem->text(), ")");
						return false;
					}
				}
				//フォント定義データを読み込み
				std::string name;
				int size = -1;
				int thick = -1;
				bool outline = false;
				bool antialiasing = false;
				elem = fd_elem->FindFirstChild("name");
				if (elem) {
					name = elem->text();
				}
				elem = fd_elem->FindFirstChild("size");
				if (elem) {
					try { size = boost::lexical_cast<int>(elem->text()); } catch (boost::bad_lexical_cast&) { PE_LOG_WARNING("sizeの値が不正です。(", elem->text(), ")"); }
				}
				elem = fd_elem->FindFirstChild("thick");
				if (elem) {
					try { thick = boost::lexical_cast<int>(elem->text()); } catch (boost::bad_lexical_cast&) { PE_LOG_WARNING("thickの値が不正です。(", elem->text(), ")"); }
				}
				elem = fd_elem->FindFirstChild("outline");
				if (elem) {
					if (elem->text() == "true" || elem->text() == "1") { outline = true; }
				}
				elem = fd_elem->FindFirstChild("antialiasing");
				if (elem) {
					if (elem->text() == "true" || elem->text() == "1") { antialiasing = true; }
				}
				//DXフォントタイプを求める
				int dx_font_type = outline ? (antialiasing ? DX_FONTTYPE_ANTIALIASING_EDGE : DX_FONTTYPE_EDGE) : (antialiasing ? DX_FONTTYPE_ANTIALIASING : DX_FONTTYPE_NORMAL);
				//DXライブラリフォントハンドルを作成
				handle_ = CreateFontToHandle(name.c_str(), size, thick, dx_font_type);
				if (handle_ < 0) {
					std::ostringstream err_str_str;
					err_str_str << "フォントハンドルの作成に失敗しました。(";
					err_str_str << "resource_id_or_path=" << resource_id_or_path << ",name=" << name << ",size=" << size << ",thick=" << thick;
					err_str_str << ",outline=" << (outline ? "true" : "false") << ",antialiasing=" << (antialiasing ? "true" : "false") << ")";
					PE_LOG_ERROR(err_str_str.str());
					return false;
				}
				size_ = GetFontSizeToHandle(handle_);
				return true;
			} else {
				PE_LOG_ERROR("フォント定義ファイルの読み込みに失敗しました。XMLファイルに\"font_definition\"エレメントが存在しません。");
				return false;
			}
		} else { //XMLファイルの読み込みに失敗
			PE_LOG_ERROR("フォント定義ファイルの読み込みに失敗しました。XMLファイルが読み込めません。");
			return false;
		}
	}

	void RFontDefinition::_Dispose() {
		if (handle_ >= 0) { DeleteFontToHandle(handle_); }
	}
}