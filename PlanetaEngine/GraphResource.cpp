#include "GraphResource.h"
#include "DxLib.h"
#include "File.h"
#include "MathUtility.h"
#include "SystemLog.h"
//#include "boost/lexical_cast.hpp"

namespace planeta {
	bool GraphResource::_Create(const std::shared_ptr<const File>& file) {
		if (file->GetStatus() != File::FileStatus::Available) { return false; }
		_handle = CreateGraphFromMem(file->GetTopPointer(), file->GetSize(), nullptr, 0, 1, 0);
		if (_handle >= 0) {
			GetGraphSize(_handle, &image_size_.x, &image_size_.y);
			if (_AdjustImageSize() == false) {
				debug::SystemLog::instance().LogError("�摜�T�C�Y�̒����Ɏ��s���܂����B", "GraphResource::_Create");
				return false;
			}
			image_area_.Set((double)image_size_.x / internal_size_.x, (double)image_size_.y / internal_size_.y);
			return true;
		} else {
			image_size_.Set(0, 0);
			internal_size_.Set(0, 0);
			image_area_.Set(0, 0);
			debug::SystemLog::instance().LogError("�摜���\�[�X�̓ǂݍ��݂Ɏ��s���܂����B", "GraphResource::_Create");
			return false;
		}
	}

	void GraphResource::_Dispose() {
		if (_handle >= 0) {
			DeleteGraph(_handle);
			_handle = -1;
		}
	}

	bool GraphResource::_AdjustImageSize() {
		bool adjust_flag = false; //�T�C�Y�������s�����ۂ��̃t���O
		internal_size_ = image_size_;
		if (image_size_.x < 8) {
			internal_size_.x = 8;
			adjust_flag = true;
		} else if (math::is2Pow(image_size_.x) == false) {
			internal_size_.x = math::getMin2Pow(image_size_.x);
			adjust_flag = true;
		}
		if (image_size_.y < 8) {
			internal_size_.y = 8;
			adjust_flag = true;
		} else if (math::is2Pow(image_size_.y) == false) {
			internal_size_.y = math::getMin2Pow(image_size_.y);
			adjust_flag = true;
		}
		if (internal_size_.x >= 2048 || internal_size_.y >= 2048) {
			debug::SystemLog::instance().LogWarning(std::string("�e�N�X�`���T�C�Y��2048�ȏ�ł��B�f�o�C�X�ɂ���Ă͕\���ł��Ȃ��\��������܂��B") + internal_size_.ToString(), "GraphResource::_AdjustImageSize");
		}
		//�T�C�Y��2�̗ݏ�łȂ�������摜�쐬
		if (adjust_flag) {
			int buf_gh = MakeScreen(internal_size_.x, internal_size_.y, true);
			if (buf_gh < 0) {
				debug::SystemLog::instance().LogError("�o�b�t�@�X�N���[���̍쐬�Ɏ��s���܂����B", "GraphResourceComponent::_AdjustImageSize");
				return false;
			}
			int cur_draw_scr = GetDrawScreen();
			bool error_flag = false;
			int texture_ghandle = MakeGraph(internal_size_.x, internal_size_.y);
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
			if (GetDrawScreenGraph(0, 0, internal_size_.x, internal_size_.y, texture_ghandle) != 0) {
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

