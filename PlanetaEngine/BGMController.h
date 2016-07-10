#pragma once

#include <memory>
#include "IBGMController.h"

namespace planeta{
	class MusicResource;
	namespace core {
		class BGMController final: public IBGMController{
		public:
			BGMController(const std::shared_ptr<MusicResource>& mr) :music_resource_(mr) {}
			~BGMController();
			bool Start(bool loop_flag)override;
			bool Stop()override;
			bool FadeIn(int frame, bool loop_flag)override;
			bool FadeOut(int frame)override;
			bool is_valid()const override;
			bool is_playing()const override;

			void Update();
			void Dispose();
			const std::shared_ptr<MusicResource> resource()const { return music_resource_; }
		private:
			std::shared_ptr<MusicResource> music_resource_;
			enum class State_{FadeIn,FadeOut,None};
			State_ state_ = State_::None;
			int fade_total_frame_ = 0;
			int fade_frame_counter_ = 0;

			bool Play_(bool loop_flag);
		};
	}
}