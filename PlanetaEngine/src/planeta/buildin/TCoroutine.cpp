#include "TCoroutine.hpp"


namespace plnt {
	void TCoroutine::update() {
		if (!coroutine_) { Start(); } else { (*coroutine_)(); }
		if (!(*coroutine_)) { if (dispose_when_coroutine_finished_) { dispose(); } else { coroutine_.release(); } }
	}

	void TCoroutine::Start() {
		coroutine_ = std::make_unique<coroutine::pull_type>([this](func_arg_type y) { UpdateProc(y); });
	}
}
