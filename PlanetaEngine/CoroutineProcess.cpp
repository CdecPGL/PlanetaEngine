#include "CoroutineProcess.h"
#include "SceneAccessorForGameProcess.h"

namespace planeta_engine {
	namespace game_processes {
		void CoroutineProcess::Update()
		{
			if (!coroutine_) { Start(); }
			else { (*coroutine_)(); }
			if (!(*coroutine_)) {
				if (dispose_when_coroutine_finished_) { scene().game_process_manager().RemoveGameProcess(id()); }
				else { coroutine_.release(); }
			}
		}

		void CoroutineProcess::Start()
		{
			coroutine_ = std::make_unique<coroutine::pull_type>([this](func_arg_type y) {UpdateProc(y); });
		}

	}
}