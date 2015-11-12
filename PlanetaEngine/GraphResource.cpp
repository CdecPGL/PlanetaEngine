#include "GraphResource.h"
#include "DxLib.h"
#include "File.h"
#include "MathUtility.h"
#include "SystemLog.h"
//#include "boost/lexical_cast.hpp"

namespace planeta_engine {
	namespace resources {
		bool GraphResource::_Create(const std::shared_ptr<const file_system::File>& file)
		{
			if (file->GetStatus() != file_system::File::FileStatus::Available) { return false; }
			_handle = CreateGraphFromMem(file->GetTopPointer(), file->GetSize(), nullptr, 0, 1, 0);
			if (_handle >= 0) {
				GetGraphSize(_handle, &_image_size.x, &_image_size.y);
				if (_AdjustImageSize() == false) {
					debug::SystemLog::instance().LogError("�摜�T�C�Y�̒����Ɏ��s���܂����B", "GraphResource::_Create");
					return false;
				}
				return true;
			}
			else { 
				_image_size.Set(0, 0);
				_internal_size.Set(0, 0);
				debug::SystemLog::instance().LogError("�摜���\�[�X�̓ǂݍ��݂Ɏ��s���܂����B", "GraphResource::_Create");
				return false;
			}
		}

		void GraphResource::_Dispose()
		{
			if (_handle >= 0) {
				DeleteGraph(_handle);
				_handle = -1;
			}
		}

		bool GraphResource::_AdjustImageSize()
		{
			bool adjust_flag = false; //�T�C�Y�������s�����ۂ��̃t���O
			_internal_size = _image_size;
			if (_image_size.x < 8) {
				_internal_size.x = 8;
				adjust_flag = true;
			}
			else if (math::is2Pow(_image_size.x) == false) {
				_internal_size.x = math::getMin2Pow(_image_size.x);
				adjust_flag = true;
			}
			if (_image_size.y < 8) {
				_internal_size.y = 8;
				adjust_flag = true;
			}
			else if (math::is2Pow(_image_size.y) == false) {
				_internal_size.y = math::getMin2Pow(_image_size.y);
				adjust_flag = true;
			}
			if (_internal_size.x >= 2048 || _internal_size.y >= 2048) {
				debug::SystemLog::instance().LogWarning(std::string("�e�N�X�`���T�C�Y��2048�ȏ�ł��B�f�o�C�X�ɂ���Ă͕\���ł��Ȃ��\��������܂��B") + _internal_size.ToString(), "GraphResource::_AdjustImageSize");
			}
			//�T�C�Y��2�̗ݏ�łȂ�������摜�쐬
			if (adjust_flag) {
				int buf_gh = MakeScreen(_internal_size.x, _internal_size.y, true);
				if (buf_gh < 0) { 
					debug::SystemLog::instance().LogError("�o�b�t�@�X�N���[���̍쐬�Ɏ��s���܂����B", "GraphResourceComponent::_AdjustImageSize");
					return false; 
				}
				int cur_draw_scr = GetDrawScreen();
				bool error_flag = false;
				int texture_ghandle = MakeGraph(_internal_size.x, _internal_size.y);
				if (texture_ghandle < 0) {
					debug::SystemLog::instance().LogError("�V�����摜�f�[�^�̍쐬�Ɏ��s���܂����B", "GraphResourceComponent::_AdjustImageSize");
					error_flag = true;
				}
				if (SetDrawScreen(buf_gh) != 0) {
					debug::SystemLog::instance().LogError("�`���̐؂�ւ��Ɏ��s���܂����B", "GraphResourceComponent::_AdjustImageSize");
					error_flag = true;
				}
				if (DrawGraph(0, 0, _handle, true) != 0) {
					debug::SystemLog::instance().LogError("�摜�f�[�^�̕`��Ɏ��s���܂����B", "GraphResourceComponent::_AdjustImageSize");
					error_flag = true;
				}
				if (GetDrawScreenGraph(0, 0, _internal_size.x, _internal_size.y, texture_ghandle) != 0) {
					debug::SystemLog::instance().LogError("�`��f�[�^�̎擾�Ɏ��s���܂����B", "GraphResourceComponent::_AdjustImageSize");
					error_flag = true;
				}
				if (SetDrawScreen(cur_draw_scr) != 0) {
					debug::SystemLog::instance().LogError("�`���̕����Ɏ��s���܂����B", "GraphResourceComponent::_AdjustImageSize");
					error_flag = true;
				}
				DeleteGraph(buf_gh);
				DeleteGraph(_handle);
				if (error_flag) { return false; }
				_handle = texture_ghandle;
			}
			return true;
		}

	}
}

