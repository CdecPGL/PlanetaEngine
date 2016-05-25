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

			//システム関数
			bool Update();

			/*新しいスクリーンを作成する(現状は一つのスクリーンにのみ対応)*/
			std::shared_ptr<Screen> CreateScreen();
			/*スクリーンを破棄する*/
			bool DisposeScreen(const std::shared_ptr<Screen>& screen);
		private:
			RenderManager() = default;
			~RenderManager() = default;

			std::shared_ptr<Screen> primary_screen_; //メインスクリーン

			bool SetDrawScreen_(const std::shared_ptr<Screen>& screen);
		};
	}
}
