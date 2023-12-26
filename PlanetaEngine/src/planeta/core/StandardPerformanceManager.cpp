#include <ctime>

#include "StandardPerformanceManager.hpp"
#include "IDebugManager.hpp"

namespace plnt {
	namespace {
		constexpr int fps_measure_interval{60};
	}

	namespace private_ {
		bool StandardPerformanceManager::Initialize() {
			_frame_count = 0;
			_start_time = util::Time::GetCurrentTime();
			CreateDebugInformationChannel("PerformanceManager");
			return true;
		}

		void StandardPerformanceManager::Finalize() { DeleteDebugInformationChannel(); }

		void StandardPerformanceManager::Update() {
			using namespace std::chrono;
			++fps_measure_count_;
			if (fps_measure_count_ % fps_measure_interval == 0) {
				auto div_time = system_clock::now() - last_measure_fps_time_;
				fps_ = 1000.0 / duration_cast<milliseconds>(div_time).count() * fps_measure_interval;
				last_measure_fps_time_ = system_clock::now();
			}
			++_frame_count;
		}

		const util::Time StandardPerformanceManager::GetCurrentTimeCount() const {
			return util::Time::GetCurrentTime() - _start_time;
		}

		void StandardPerformanceManager::DebugInfotmationAddHandler(IDebugInformationAdder &di_adder) {
			di_adder.AddLineV("FPS:", fps_);
			di_adder.AddLineV("経過フレーム:", _frame_count);
			di_adder.AddLineV("経過時間:", (util::Time::GetCurrentTime() - _start_time).ToString());
		}
	}
}
