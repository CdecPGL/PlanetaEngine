#pragma once

#include "RenderingManager.h"

namespace planeta {
	namespace private_{
		class StandardRenderingManager final : public RenderingManager{
		public:
			StandardRenderingManager() = default;
			~StandardRenderingManager() = default;

			bool Initialize() override;
			void Finalize() override;

			//システム関数
			bool Update()override;

			/*新しいスクリーンを作成する(現状は一つのスクリーンにのみ対応)*/
			std::shared_ptr<Screen> CreateScreen()override;
			/*スクリーンを破棄する*/
			bool DisposeScreen(const std::shared_ptr<Screen>& screen)override;
		private:
			std::shared_ptr<Screen> primary_screen_; //メインスクリーン

			bool SetDrawScreen_(const std::shared_ptr<Screen>& screen);
		};
	}
}
