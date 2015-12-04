#pragma once

namespace planeta_engine {
	namespace core {
		class Screen {
		public:
			explicit Screen(int dx_handle,bool is_primary):dx_screen_handle_(dx_handle),is_primary_(is_primary) {}
			int GetHandle()const { return dx_screen_handle_; }
			bool is_primary()const { return is_primary_; }
			bool is_active()const { return is_active_; }
		private:
			int dx_screen_handle_ = -1;
			bool is_primary_ = false;
			bool is_active_ = false;
		};
	}
}