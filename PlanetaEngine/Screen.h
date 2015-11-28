#pragma once

namespace planeta_engine {
	namespace core {
		class Screen {
		public:
			explicit Screen(int dx_handle):dx_screen_handle_(dx_handle) {}
			int GetHandle()const { return dx_screen_handle_; }
		private:
			int dx_screen_handle_ = -1;
		};
	}
}