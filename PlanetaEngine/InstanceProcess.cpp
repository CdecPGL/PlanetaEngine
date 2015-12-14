#include "InstanceProcess.h"
#include "SceneAccessorForGameProcess.h"

namespace planeta_engine {
	namespace game_processes {

		void InstantProcess::Update()
		{
			function_();
			if (number_of_times_implemented_ > 0) {
				++count_of_times_implemented_;
				if (number_of_times_implemented_ >= count_of_times_implemented_) {
					//���s�񐔂ɒB�����̂Ńv���Z�X���폜����
					Dispose();
				}
			}
		}

	}
}