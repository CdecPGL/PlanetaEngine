#include "RFontDefinition.h"

#include <windows.h>
#include <sstream>
#include "boost/lexical_cast.hpp"
#include "DxLib.h"
#include "MakeResource.h"
#include "RXml.h"
#include "File.h"
#include "FileSystemManager.h"
#include "SystemLog.h"
#include "SystemVariables.h"
#include "FileSystemManager.h"
#include "FileAccessor.h"

namespace planeta {
	bool RFontDefinition::_Create(const File& file) {
		if (file.GetStatus() != File::FileStatus::Available) { return false; }
		auto file_accessor = FileSystemManager::instance().GetFileAccessor(private_::system_variables::file_system::ResourceFileAccessorID);
		auto xml = private_::MakeResource<RXml>();
		if (xml->Create(file)) {
			auto root = xml->GetRootElement();
			auto fd_elem = root->FindFirstChild("font_definition");
			if (fd_elem) {
				auto elem = fd_elem->FindFirstChild("file_name");
				//フォントファイルが指定されていたら読み込み
				HANDLE win_font_handle = 0; //フォントのハンドル
				std::string file_name = "ファイル読み込み無し";
				if (elem) {
					file_name = elem->text();
					//フォントファイルを読み込み
					auto font_file = file_accessor->LoadFile(elem->text());
					if (font_file == nullptr) {
						PE_LOG_ERROR("フォントファイルの読み込みに失敗しました。(", elem->text(), ")");
						return false;
					}
					//フォントを読み込み
					DWORD num;
					win_font_handle = AddFontMemResourceEx(const_cast<unsigned char*>(font_file->GetTopPointer()), font_file->GetSize(), nullptr, &num);
					if (win_font_handle == 0) {
						PE_LOG_ERROR("フォントの読み込みに失敗しました。(", elem->text(), ")");
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
					err_str_str << "file_name=" << file_name << ",name=" << name << ",size=" << size << ",thick=" << thick;
					err_str_str << ",outline=" << (outline ? "true" : "false") << ",antialiasing=" << (antialiasing ? "true" : "false") << ")";
					PE_LOG_ERROR(err_str_str.str());
					return false;
				}
				size_ = GetFontSizeToHandle(handle_);
				//フォントデータを読み込んでいたら削除する
				if (win_font_handle) { RemoveFontMemResourceEx(win_font_handle); }
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