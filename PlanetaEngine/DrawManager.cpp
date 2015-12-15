#include <cassert>
#include "DrawManager.h"
#include "DxLib.h"
#include "DXUtility.h"
#include "MathUtility.h"
#include "Screen.h"
#include "ConfigData.h"

namespace planeta_engine {
	namespace core {
		bool planeta_engine::core::DrawManager::Initialize()
		{
			SetDrawScreen(DX_SCREEN_BACK); //�o�b�N�X�N���[����`��Ώۂ�
			SetupCamera_Ortho((float)config_data::engine::DrawSize().y); //���ˉe�J������ݒ�
			SetUseLighting(false); //���C�e�B���O�v�Z���s��Ȃ�
			SetUseSetDrawScreenSettingReset(false); //�X�N���[���ύX���ɃJ�������̃��Z�b�g���s��Ȃ��B
			camera_posision_.Set(config_data::engine::DrawSize().x / 2, config_data::engine::DrawSize().y / 2);
			return true;
		}

		bool planeta_engine::core::DrawManager::Finalize()
		{
			return true;
		}

		bool DrawManager::Update()
		{
			//�f�o�b�O�p
			//�O���b�h��(32���ƂɈ���)
			//for (int i = 0; i <= 640; i += 32) {
			//	DrawWire({ Vector2D<double>(i,0),Vector2D<double>(i,480) }, 1, Color::Green());
			//}
			//for (int i = 0; i <= 480; i += 32) {
			//	DrawWire({ Vector2D<double>(0,i),Vector2D<double>(640,i) }, 1, Color::Green());
			//}
			//
			////�g
			//DrawWire({ Vector2D<double>(0,0),Vector2D<double>(640,0),Vector2D<double>(640,480),Vector2D<double>(0,480),Vector2D<double>(0,0) }, 1,Color::Red());

			//�}�[�J�[
			//DrawWire({ Vector2D<double>(0,0),Vector2D<double>(32,32) }, 1, Color::Blue());
			//DrawWire({ Vector2D<double>(640,0),Vector2D<double>(608,32) }, 1, Color::Yellow());
			//DrawWire({ Vector2D<double>(640,480),Vector2D<double>(608,448) }, 1, Color::Magenta());
			//DrawWire({ Vector2D<double>(0,480),Vector2D<double>(32,448) }, 1, Color::Cyan());
			//�X�N���[���ɕ`��
			primary_screen_->HandleDrawReservations();
			//��ʍX�V
			if (!(ScreenFlip() == 0 && ClearDrawScreen() == 0)) { return false; }
			return true;
		}

		void DrawManager::SetCameraPosition(const Vector2D<double>& position)
		{
			if (!math::DoubleIsEqual(camera_posision_.x, position.x) || !math::DoubleIsEqual(camera_posision_.y, position.y)) {
				camera_posision_ = position;
				SetCameraPositionAndAngle(VGet((float)position.x, (float)position.y, GetCameraPosition().z), GetCameraAngleVRotate(), GetCameraAngleHRotate(), GetCameraAngleTRotate());
			}
		}

		void DrawManager::SetCameraRotationRad(double rotation_rad)
		{
			if (!math::DoubleIsEqual(camera_rotation_rad_,rotation_rad)) {
				camera_rotation_rad_ = rotation_rad;
				SetCameraPositionAndAngle(GetCameraPosition(), GetCameraAngleVRotate(), GetCameraAngleHRotate(), (float)rotation_rad);
			}
		}

		void DrawManager::SetCameraScale(double scale)
		{
			if (!math::DoubleIsEqual(camera_scale_, scale)) {
				camera_scale_ = scale;
				SetupCamera_Ortho(480.0f / (float)scale);
			}
		}

		std::shared_ptr<Screen> DrawManager::CreateScreen() {
			if (primary_screen_) { 
				assert(false); 
				return nullptr;
			}
			primary_screen_ = std::make_shared<Screen>(DX_SCREEN_BACK);
			return primary_screen_;
		}

		bool DrawManager::SetDrawScreen_(const std::shared_ptr<Screen>& screen) {
			int gh = screen->GetHandle();
			SetDrawScreen(gh);
			return true;
		}

		bool DrawManager::DisposeScreen(const std::shared_ptr<Screen>& screen) {
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

