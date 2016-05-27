#include "DxLib.h"

#include "InitFunctions.h"
#include "SystemVariables.h"
#include "EngineConfigData.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeDxLib() {
				//DX���C�u�����̏�����
				//////////////////////////////////////////////////////////////////////////
				//���O�o�͐��ύX
				SetApplicationLogSaveDirectory(system_variables::file_system::LogDirectory.c_str());
				//�E�C���h�E���[�h�ݒ�
				ChangeWindowMode(engine_config::engine::WindowMode());
				//�E�C���h�E�^�C�g���ݒ�(�Q�[���^�C�g��+�Q�[���o�[�W����)
				SetMainWindowText((engine_config::game::GameTitle() + " v" + engine_config::game::VersionString()).c_str());
				//�`��T�C�Y�ƃJ���[�r�b�g�[�x�ݒ�
				SetGraphMode(engine_config::engine::DrawSize().x, engine_config::engine::DrawSize().y, engine_config::engine::ColorBitDepth());
				//�K�v�Ȃ�E�C���h�E�T�C�Y�ɍ��킹�Ċg�k
				{
					const auto& ds = engine_config::engine::DrawSize();
					const auto& ws = engine_config::engine::WindowSize();
					if (ds.x != ws.x || ds.y != ws.y) {
						SetWindowSizeExtendRate((double)ws.x / ds.x, (double)ws.y / ds.y);
					}
				}
				if (DxLib_Init() == 0) {
					return{ true,[] {DxLib_End(); } };
				} else { 
					PE_LOG_FATAL("DX���C�u�����̏������Ɏ��s���܂����B");
					return{ false,[] {} };
				}
			}
		}
	}
}