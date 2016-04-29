#pragma once

#include <functional>
#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForTask;
	namespace core {
		class TaskManagerConnection {
		public:
			TaskManagerConnection(std::function<bool()>&& pauser, std::function<bool()>&& resumer, std::function<void()>&& disposer, const utility::WeakPointer<SceneAccessorForTask>& scene_accessor)
				:pauser_(pauser), resumer_(resumer), disposer_(disposer), scene_accessor_(scene_accessor) {};
			bool Pause();
			bool Resume();
			void Dispose();
			utility::WeakPointer<SceneAccessorForTask>& ReferenceSceneAccessor();
		private:
			std::function<bool()> pauser_;
			std::function<bool()> resumer_;
			std::function<void()> disposer_;
			utility::WeakPointer<SceneAccessorForTask> scene_accessor_;
		};
	}
}