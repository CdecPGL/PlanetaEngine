#include <cassert>
#include "RenderManager.h"
#include "DxLib.h"
#include "Screen.h"
#include "SystemLog.h"

namespace planeta {
	namespace core {
		bool planeta::core::RenderManager::Initialize()
		{
			SetDrawScreen(DX_SCREEN_BACK); //�o�b�N�X�N���[����`��Ώۂ�
			SetUseLighting(false); //���C�e�B���O�v�Z���s��Ȃ�
			SetUseSetDrawScreenSettingReset(false); //�X�N���[���ύX���ɃJ�������̃��Z�b�g���s��Ȃ��B
			SetDrawMode(DX_DRAWMODE_BILINEAR); //�o�C���j�A�@�ŕ`��
			return true;
		}

		void planeta::core::RenderManager::Finalize()
		{
		}

		bool RenderManager::Update()
		{
			////�f�o�b�O�p
			////�O���b�h��(32���ƂɈ���)
			//for (int i = 0; i <= 640; i += 32) {
			//	DrawWire({ Vector2Dd(i,0),Vector2Dd(i,480) }, 1, Color::Green());
			//}
			//for (int i = 0; i <= 480; i += 32) {
			//	DrawWire({ Vector2Dd(0,i),Vector2Dd(640,i) }, 1, Color::Green());
			//}
			//
			//////�g
			//DrawWire({ Vector2Dd(0,0),Vector2Dd(640,0),Vector2Dd(640,480),Vector2Dd(0,480),Vector2Dd(0,0) }, 1,Color::Red());

			////�}�[�J�[
			////DrawWire({ Vector2Dd(0,0),Vector2Dd(32,32) }, 1, Color::Blue());
			////DrawWire({ Vector2Dd(640,0),Vector2Dd(608,32) }, 1, Color::Yellow());
			////DrawWire({ Vector2Dd(640,480),Vector2Dd(608,448) }, 1, Color::Magenta());
			////DrawWire({ Vector2Dd(0,480),Vector2Dd(32,448) }, 1, Color::Cyan());
			//�X�N���[���ɕ`��
			primary_screen_->HandleDrawReservations();
			//��ʍX�V
			if (!(ScreenFlip() == 0 && ClearDrawScreen() == 0)) { return false; }
			return true;
		}

		std::shared_ptr<Screen> RenderManager::CreateScreen() {
			if (primary_screen_) { 
				assert(false); 
				return nullptr;
			}
			primary_screen_ = std::make_shared<Screen>(DX_SCREEN_BACK);
			return primary_screen_;
		}

		bool RenderManager::SetDrawScreen_(const std::shared_ptr<Screen>& screen) {
			int gh = screen->GetHandle();
			SetDrawScreen(gh);
			return true;
		}

		bool RenderManager::DisposeScreen(const std::shared_ptr<Screen>& screen) {
			if (primary_screen_ && primary_screen_.get() == screen.get()) {
				primary_screen_.reset();
				return true;
			} else {
				assert(false);
				return true;
			}
		}

	}
}

