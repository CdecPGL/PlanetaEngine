#include <ctime>

#include "StandardPerformanceManager.hpp"
#include "i_debug_manager.hpp"

namespace plnt {
	namespace {
		constexpr int fps_measure_interval{60};
	}

	namespace private_ {
		bool StandardPerformanceManager::initialize() {
			_frame_count = 0;
			_start_time = util::time::get_current_time();
			create_debug_information_channel("PerformanceManager");
			return true;
		}

		void StandardPerformanceManager::finalize() { delete_debug_information_channel(); }

		void StandardPerformanceManager::update() {
			using namespace std::chrono;
			++fps_measure_count_;
			if (fps_measure_count_ % fps_measure_interval == 0) {
				auto div_time = system_clock::now() - last_measure_fps_time_;
				fps_ = 1000.0 / duration_cast<milliseconds>(div_time).count() * fps_measure_interval;
				last_measure_fps_time_ = system_clock::now();
			}
			++_frame_count;
		}

		const util::time StandardPerformanceManager::get_current_time_count() const {
			return util::time::get_current_time() - _start_time;
		}

		void StandardPerformanceManager::debug_information_add_handler(i_debug_information_adder &di_adder) {
			di_adder.add_line_v("FPS:", fps_);
			di_adder.add_line_v("経過フレーム:", _frame_count);
			di_adder.add_line_v("経過時間:", (util::time::get_current_time() - _start_time).to_string());
		}
	}
}
