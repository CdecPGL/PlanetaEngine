#include "screen.hpp"
#include "log_utility.hpp"

namespace plnt::private_ {
	screen::screen() : is_valid_(true) { }

	void screen::handle_draw_reservations() {
		if (!is_valid()) { PE_LOG_ERROR("無効なスクリーンに描画を行おうとしました。"); }
		for (int i = 0; i < draw_reservation_count_; ++i) { draw_reservations_[i](); }
		reset_all_reservations();
	}

	void screen::reset_all_reservations() {
		draw_reservations_.clear();
		draw_reservation_count_ = 0;
	}

	void screen::reserve_draw(const std::function<void()> &drawer) {
		++draw_reservation_count_;
		if (static_cast<signed>(draw_reservations_.size()) < draw_reservation_count_) { draw_reservations_.push_back(drawer); }
		draw_reservations_[draw_reservation_count_ - 1] = drawer;
	}

	bool screen::is_valid() const { return is_valid_; }

	void screen::invalidate() {
		is_valid_ = false;
		reset_all_reservations();
	}
}
