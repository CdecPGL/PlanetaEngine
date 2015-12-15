#pragma once

#include <vector>
#include <functional>

namespace planeta_engine {
	namespace core {
		class Screen {
		public:
			explicit Screen(int dx_handle):dx_screen_handle_(dx_handle),is_valid_(true){}
			int GetHandle()const { return dx_screen_handle_; }
			void HandleDrawReservations();
			void ResetAllReservations();
			void ReserveDraw(const std::function<void()>& drawer);
			bool is_valid()const { return is_valid_; }
			void Invalidate();
		private:
			bool is_valid_;
			int dx_screen_handle_ = -1;
			std::vector<std::function<void()>> draw_reservations_;
			int draw_reservation_count_ = 0;
		};
	}
}