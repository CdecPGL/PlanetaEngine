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
	bool RFontDefinition::_Create(const std::shared_ptr<const File>& file) {
		if (file->GetStatus() != File::FileStatus::Available) { return false; }
		auto file_accessor = FileSystemManager::instance().GetFileAccessor(core::system_variables::file_system::ResourceFileAccessorID);
		auto xml = core::MakeResource<RXml>();
		if (xml->Create(file)) {
			auto root = xml->GetRootElement();
			auto fd_elem = root->FindFirstChild("font_definition");
			if (fd_elem) {
				auto elem = fd_elem->FindFirstChild("file_name");
				//�t�H���g�t�@�C�����w�肳��Ă�����ǂݍ���
				HANDLE win_font_handle = 0; //�t�H���g�̃n���h��
				std::string file_name = "�t�@�C���ǂݍ��ݖ���";
				if (elem) {
					file_name = elem->text();
					//�t�H���g�t�@�C����ǂݍ���
					auto font_file = file_accessor->LoadFile(elem->text());
					if (font_file == nullptr) {
						PE_LOG_ERROR("�t�H���g�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(", elem->text(), ")");
						return false;
					}
					//�t�H���g��ǂݍ���
					DWORD num;
					win_font_handle = AddFontMemResourceEx(const_cast<unsigned char*>(font_file->GetTopPointer()), font_file->GetSize(), nullptr, &num);
					if (win_font_handle == 0) {
						PE_LOG_ERROR("�t�H���g�̓ǂݍ��݂Ɏ��s���܂����B(", elem->text(), ")");
						return false;
					}
				}
				//�t�H���g��`�f�[�^��ǂݍ���
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
					try { size = boost::lexical_cast<int>(elem->text()); } catch (boost::bad_lexical_cast&) { PE_LOG_WARNING("size�̒l���s���ł��B(", elem->text(), ")"); }
				}
				elem = fd_elem->FindFirstChild("thick");
				if (elem) {
					try { thick = boost::lexical_cast<int>(elem->text()); } catch (boost::bad_lexical_cast&) { PE_LOG_WARNING("thick�̒l���s���ł��B(", elem->text(), ")"); }
				}
				elem = fd_elem->FindFirstChild("outline");
				if (elem) {
					if (elem->text() == "true" || elem->text() == "1") { outline = true; }
				}
				elem = fd_elem->FindFirstChild("antialiasing");
				if (elem) {
					if (elem->text() == "true" || elem->text() == "1") { antialiasing = true; }
				}
				//DX�t�H���g�^�C�v�����߂�
				int dx_font_type = outline ? (antialiasing ? DX_FONTTYPE_ANTIALIASING_EDGE : DX_FONTTYPE_EDGE) : (antialiasing ? DX_FONTTYPE_ANTIALIASING : DX_FONTTYPE_NORMAL);
				//DX���C�u�����t�H���g�n���h�����쐬
				handle_ = CreateFontToHandle(name.c_str(), size, thick, dx_font_type);
				if (handle_ < 0) {
					std::ostringstream err_str_str;
					err_str_str << "�t�H���g�n���h���̍쐬�Ɏ��s���܂����B(";
					err_str_str << "file_name=" << file_name << ",name=" << name << ",size=" << size << ",thick=" << thick;
					err_str_str << ",outline=" << (outline ? "true" : "false") << ",antialiasing=" << (antialiasing ? "true" : "false") << ")";
					PE_LOG_ERROR(err_str_str.str());
					return false;
				}
				size_ = GetFontSizeToHandle(handle_);
				//�t�H���g�f�[�^��ǂݍ���ł�����폜����
				if (win_font_handle) { RemoveFontMemResourceEx(win_font_handle); }
				return true;
			} else {
				PE_LOG_ERROR("�t�H���g��`�t�@�C���̓ǂݍ��݂Ɏ��s���܂����BXML�t�@�C����\"font_definition\"�G�������g�����݂��܂���B");
				return false;
			}
		} else { //XML�t�@�C���̓ǂݍ��݂Ɏ��s
			PE_LOG_ERROR("�t�H���g��`�t�@�C���̓ǂݍ��݂Ɏ��s���܂����BXML�t�@�C�����ǂݍ��߂܂���B");
			return false;
		}
	}

	void RFontDefinition::_Dispose() {
		if (handle_ >= 0) { DeleteFontToHandle(handle_); }
	}
}