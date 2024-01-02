#pragma once

#include <vector>
#include <functional>

namespace plnt::private_ {
	class screen {
	public:
		explicit screen();
		void handle_draw_reservations();
		void reset_all_reservations();
		void reserve_draw(const std::function<void()> &drawer);
		[[nodiscard]] bool is_valid() const;
		void invalidate();

	private:
		bool is_valid_;
		std::vector<std::function<void()>> draw_reservations_;
		int draw_reservation_count_ = 0;
	};
}
