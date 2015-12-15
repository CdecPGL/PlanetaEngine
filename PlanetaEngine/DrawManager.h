#pragma once
#include <vector>
#include <array>
#include <memory>
#include <deque>
#include "PointerSingletonTemplate.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace core{
		class Screen;
		class DrawManager final : public utility::PointerSingletonTemplate<DrawManager>{
			friend utility::PointerSingletonTemplate<DrawManager>;
		public:
			bool Initialize() override;
			bool Finalize() override;

			//�V�X�e���֐�
			bool Update();
			//�J�����֐�
			/*�J�����ʒu��ݒ�*/
			void SetCameraPosition(const Vector2D<double>& position);
			/*�J������]�x��ݒ�*/
			void SetCameraRotationRad(double rotation_rad);
			/*�J�����g��x��ݒ�*/
			void SetCameraScale(double scale);

			/*�V�����X�N���[�����쐬����(����͈�̃X�N���[���ɂ̂ݑΉ�)*/
			std::shared_ptr<Screen> CreateScreen();
			/*�X�N���[����j������*/
			bool DisposeScreen(const std::shared_ptr<Screen>& screen);
		private:
			DrawManager() = default;
			~DrawManager() = default;

			Vector2D<double> camera_posision_;
			double camera_rotation_rad_ = 0.0;
			double camera_scale_ = 1.0;

			std::shared_ptr<Screen> primary_screen_; //���C���X�N���[��

			bool SetDrawScreen_(const std::shared_ptr<Screen>& screen);
		};
	}
}
