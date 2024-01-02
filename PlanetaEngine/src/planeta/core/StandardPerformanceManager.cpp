#include "StandardPerformanceManager.hpp"
#include "i_debug_manager.hpp"

namespace plnt {
	namespace {
		constexpr int fps_measure_interval{60};
	}

	namespace private_ {
		bool standard_performance_manager::initialize() {
			frame_count_ = 0;
			start_time_ = util::time::get_current_time();
			create_debug_information_channel("PerformanceManager");
			return true;
		}

		void standard_performance_manager::finalize() { delete_debug_information_channel(); }

		void standard_performance_manager::update() {
			using namespace std::chrono;
			++fps_measure_count_;
			if (fps_measure_count_ % fps_measure_interval == 0) {
				const auto div_time = system_clock::now() - last_measure_fps_time_;
				fps_ = 1000.0 / static_cast<double>(duration_cast<milliseconds>(div_time).count()) *
					fps_measure_interval;
				last_measure_fps_time_ = system_clock::now();
			}
			++frame_count_;
		}

		const util::time standard_performance_manager::get_current_time_count() const {
			return util::time::get_current_time() - start_time_;
		}

		void standard_performance_manager::debug_information_add_handler(i_debug_information_adder &di_adder) {
			di_adder.add_line_v("FPS:", fps_);
			di_adder.add_line_v("経過フレーム:", frame_count_);
			di_adder.add_line_v("経過時間:", (util::time::get_current_time() - start_time_).to_string());
		}
	}
}
