#include "InitEnd.h"
#include <string>
#include "DxLib.h"
#include "SystemLog.h"
#include "SystemVariables.h"
#include "ConfigData.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace core {
		namespace init_end {
			bool InitDxLibrary() {
				//���O�o�͐��ύX
				SetApplicationLogSaveDirectory(system_variables::LogDirectory.c_str());
				//�E�C���h�E���[�h�ݒ�
				ChangeWindowMode(config_data::engine::WindowMode());
				//�E�C���h�E�^�C�g���ݒ�(�Q�[���^�C�g��+�Q�[���o�[�W����)
				SetMainWindowText((config_data::game::GameTitle() + " v" + config_data::game::VersionString()).c_str());
				//�`��T�C�Y�ƃJ���[�r�b�g�[�x�ݒ�
				SetGraphMode(config_data::engine::DrawSize().x, config_data::engine::DrawSize().y, config_data::engine::ColorBitDepth());
				//�K�v�Ȃ�E�C���h�E�T�C�Y�ɍ��킹�Ċg�k
				{
					const auto& ds = config_data::engine::DrawSize();
					const auto& ws = config_data::engine::WindowSize();
					if (ds.x != ws.x || ds.y != ws.y) {
						SetWindowSizeExtendRate((double)ws.x / ds.x, (double)ws.y / ds.y);
					}
				}
				if (DxLib_Init() < 0) {
					debug::SystemLog::instance().LogError("DX���C�u�����̏������Ɏ��s���܂����B",__FUNCTION__);
					return false;
				}
				else {
					debug::SystemLog::instance().LogMessage("DX���C�u�����̏������ɐ������܂����B", __FUNCTION__);
					return true;
				}
			}

			bool EndDxLibrary() {
				debug::SystemLog::instance().LogMessage("DX���C�u�����̏I���������s���܂����B", __FUNCTION__);
				DxLib_End();
				return true;
			}
		}
	}
}