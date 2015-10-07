#include "FontDefinitionResource.h"

#include <windows.h>
#include <sstream>
#include "boost/lexical_cast.hpp"
#include "DxLib.h"
#include "MakeResource.h"
#include "XMLResource.h"
#include "File.h"
#include "FileLoadManager.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace resources {

		bool FontDefinitionResource::_Create(const std::shared_ptr<file_system::File>& file)
		{
			if (file->GetStatus() != file_system::File::FileStatus::Available) { return false; }
			auto xml = core::MakeResource<XMLResource>();

			if (xml->Create(file)) {
				auto root = xml->GetRootElement();
				auto fd_elem = root->FindFirstChild("font_definition");
				if(fd_elem)
				{
					auto elem = fd_elem->FindFirstChild("file_name");
					//�t�H���g�t�@�C�����w�肳��Ă�����ǂݍ���
					HANDLE win_font_handle = 0; //�t�H���g�̃n���h��
					std::string file_name = "�t�@�C���ǂݍ��ݖ���";
					if (elem) {
						file_name = elem->text();
						//�t�H���g�t�@�C����ǂݍ���
						auto font_file = file_system::FileLoadManager::instance().LoadFile(elem->text());
						if (font_file == nullptr) {
							debug::SystemLog::instance().LogError(std::string("�t�H���g�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(" + elem->text() + ")"), __FUNCTION__);
							return false;
						}
						//�t�H���g��ǂݍ���
						DWORD num;
						win_font_handle = AddFontMemResourceEx(const_cast<void*>(font_file->GetTopPointer()), font_file->GetSize(), nullptr, &num);
						if (win_font_handle == 0) {
							debug::SystemLog::instance().LogError(std::string("�t�H���g�̓ǂݍ��݂Ɏ��s���܂����B(") + elem->text() + ")", __FUNCTION__);
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
						try { size = boost::lexical_cast<int>(elem->text()); }
						catch (boost::bad_lexical_cast&) { debug::SystemLog::instance().LogWarning(std::string("size�̒l���s���ł��B(") + elem->text() + ")", __FUNCTION__); }
					}
					elem = fd_elem->FindFirstChild("thick");
					if (elem) {
						try { thick = boost::lexical_cast<int>(elem->text()); }
						catch (boost::bad_lexical_cast&) { debug::SystemLog::instance().LogWarning(std::string("thick�̒l���s���ł��B(") + elem->text() + ")", __FUNCTION__); }
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
						debug::SystemLog::instance().LogError(err_str_str.str(), __FUNCTION__);
						return false;
					}
					//�t�H���g�f�[�^��ǂݍ���ł�����폜����
					if (win_font_handle) { RemoveFontMemResourceEx(win_font_handle); }
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("�t�H���g��`�t�@�C���̓ǂݍ��݂Ɏ��s���܂����BXML�t�@�C����\"font_definition\"�G�������g�����݂��܂���B", __FUNCTION__);
					return false;
				}
			}
			else { //XML�t�@�C���̓ǂݍ��݂Ɏ��s
				debug::SystemLog::instance().LogError("�t�H���g��`�t�@�C���̓ǂݍ��݂Ɏ��s���܂����BXML�t�@�C�����ǂݍ��߂܂���B", __FUNCTION__);
				return false;
			}
		}

		void FontDefinitionResource::_Dispose()
		{
			if (handle_ >= 0) { DeleteFontToHandle(handle_); }
		}

	}
}