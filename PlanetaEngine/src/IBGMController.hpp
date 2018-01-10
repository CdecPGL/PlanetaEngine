#pragma once

namespace plnt {
	class IBGMController {
	public:
		virtual ~IBGMController() = default;
		virtual bool Start(bool loop_flag) = 0;
		virtual bool Stop() = 0;
		virtual bool FadeIn(int frame, bool loop_flag) = 0;
		virtual bool FadeOut(int frame) = 0;
		virtual bool is_valid()const = 0;
		virtual bool is_playing()const = 0;
	};
}