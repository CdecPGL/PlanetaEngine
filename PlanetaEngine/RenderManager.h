#pragma once
#include <vector>
#include <array>
#include <memory>
#include <deque>
#include "SingletonTemplate.h"

namespace planeta_engine {
	namespace core{
		class Screen;
		class RenderManager final : public utility::SingletonTemplate<RenderManager>{
			friend utility::SingletonTemplate<RenderManager>;
		public:
			bool Initialize() override;
			bool Finalize() override;

			//�V�X�e���֐�
			bool Update();

			/*�V�����X�N���[�����쐬����(����͈�̃X�N���[���ɂ̂ݑΉ�)*/
			std::shared_ptr<Screen> CreateScreen();
			/*�X�N���[����j������*/
			bool DisposeScreen(const std::shared_ptr<Screen>& screen);
		private:
			RenderManager() = default;
			~RenderManager() = default;

			std::shared_ptr<Screen> primary_screen_; //���C���X�N���[��

			bool SetDrawScreen_(const std::shared_ptr<Screen>& screen);
		};
	}
}
