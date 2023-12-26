#pragma once

#include <vector>
#include <functional>

namespace plnt {
	namespace private_ {
		class Screen {
		public:
			explicit Screen();
			void HandleDrawReservations();
			void ResetAllReservations();
			void ReserveDraw(const std::function<void()> &drawer);
			bool is_valid() const;
			void Invalidate();

		private:
			bool is_valid_;
			std::vector<std::function<void()>> draw_reservations_;
			int draw_reservation_count_ = 0;
		};
	}
}
