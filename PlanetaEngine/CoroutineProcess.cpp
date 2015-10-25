#include "CoroutineProcess.h"
#include "ISceneAccessForGameProcess.h"
#include "IGameProcessManagerAccessor.h"

namespace planeta_engine {
	namespace game_processes {
		void CoroutineProcess::Update()
		{
			if (!coroutine_) { Start(); }
			else {
				if (*coroutine_) { (*coroutine_)(); }
				else {
					if (dispose_when_coroutine_finished_) { scene()->game_process_manager().RemoveGameProcess(id()); }
				}
			}
		}

		void CoroutineProcess::Start()
		{
			coroutine_ = std::make_unique<coroutine::pull_type>([this](func_arg_type y) {UpdateProc(y); });
		}

	}
}