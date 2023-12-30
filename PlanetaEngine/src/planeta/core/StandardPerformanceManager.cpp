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
			_start_time = util::Time::GetCurrentTime();
			CreateDebugInformationChannel("PerformanceManager");
			return true;
		}

		void StandardPerformanceManager::finalize() { DeleteDebugInformationChannel(); }

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

		const util::Time StandardPerformanceManager::get_current_time_count() const {
			return util::Time::GetCurrentTime() - _start_time;
		}

		void StandardPerformanceManager::DebugInfotmationAddHandler(i_debug_information_adder &di_adder) {
			di_adder.add_line_v("FPS:", fps_);
			di_adder.add_line_v("経過フレーム:", _frame_count);
			di_adder.add_line_v("経過時間:", (util::Time::GetCurrentTime() - _start_time).to_string());
		}
	}
}
