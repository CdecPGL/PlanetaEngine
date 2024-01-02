#pragma once

#include <functional>

namespace plnt::private_ {
	class task_manager_connection {
	public:
		task_manager_connection(std::function<bool()> &&pauser, std::function<bool()> &&resumer,
		                        std::function<void()> &&disposer)
			: pauser_(std::move(pauser)), resumer_(std::move(resumer)), disposer_(std::move(disposer)) {}

		// NOLINTNEXTLINE(modernize-use-nodiscard)
		bool pause() const;
		// NOLINTNEXTLINE(modernize-use-nodiscard)
		bool resume() const;
		void dispose() const;

	private:
		std::function<bool()> pauser_;
		std::function<bool()> resumer_;
		std::function<void()> disposer_;
	};
}
