#include "planeta/core/Screen.hpp"
#include "planeta/core/LogUtility.hpp"

namespace plnt {
	namespace private_ {

		Screen::Screen() :is_valid_(true) {

		}

		void Screen::HandleDrawReservations() {
			if (!is_valid()) {
				PE_LOG_ERROR("無効なスクリーンに描画を行おうとしました。");
			}
			for (int i = 0; i < draw_reservation_count_; ++i) {
				draw_reservations_[i]();
			}
			ResetAllReservations();
		}

		void Screen::ResetAllReservations() {
			draw_reservations_.clear();
			draw_reservation_count_ = 0;
		}

		void Screen::ReserveDraw(const std::function<void()>& drawer) {
			++draw_reservation_count_;
			if ((signed)draw_reservations_.size() < draw_reservation_count_) { draw_reservations_.push_back(drawer); }
			draw_reservations_[draw_reservation_count_ - 1] = drawer;
		}

		bool Screen::is_valid() const {
			return is_valid_;
		}

		void Screen::Invalidate() {
			is_valid_ = false;
			ResetAllReservations();
		}

	}
}