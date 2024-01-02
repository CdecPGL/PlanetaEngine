#pragma once

#include <chrono>

#include "perfoamance_manager.hpp"

namespace plnt::private_ {
	//起動からの経過フレームと経過時間をカウントする
	class standard_performance_manager final : public performance_manager {
	public:
		standard_performance_manager() = default;
		bool initialize() override;
		void finalize() override;

		void update() override;
		[[nodiscard]] size_t get_current_frame_count() const override { return frame_count_; }
		[[nodiscard]] const util::time get_current_time_count() const override;
		void debug_information_add_handler(i_debug_information_adder &di_adder) override;

	private:
		size_t frame_count_ = 0;
		util::time start_time_;
		std::chrono::system_clock::time_point last_measure_fps_time_;
		int fps_measure_count_ = 0;
		double fps_ = 0;
	};
}
