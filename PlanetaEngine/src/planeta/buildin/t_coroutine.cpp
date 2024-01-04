#include "t_coroutine.hpp"


namespace plnt {
	void t_coroutine::update() {
		if (!coroutine_) { start(); } else { (*coroutine_)(); }
		if (!*coroutine_) { if (dispose_when_coroutine_finished_) { dispose(); } else { coroutine_.release(); } }
	}

	void t_coroutine::start() {
		coroutine_ = std::make_unique<coroutine::pull_type>([this](func_arg_type y) { update_proc(y); });
	}
}
