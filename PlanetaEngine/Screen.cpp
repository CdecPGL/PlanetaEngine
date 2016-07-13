#include "Screen.h"
#include "DxLib.h"
#include "SystemLog.h"

namespace planeta {
	namespace core {
		void Screen::HandleDrawReservations() {
			if (!is_valid()) {
				PE_LOG_ERROR("無効なスクリーンに描画を行おうとしました。");
			}
			//必要なら対象のスクリーンに切り替え
			if (GetDrawScreen() != dx_screen_handle_) { SetDrawScreen(dx_screen_handle_); }			for (int i = 0; i < draw_reservation_count_; ++i) {
				draw_reservations_[i]();
			}
			draw_reservation_count_ = 0;
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

		void Screen::Invalidate() {
			is_valid_ = false;
			ResetAllReservations();
		}

	}
}