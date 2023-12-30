#pragma once

#include "Signal.hpp"
#include "object.hpp"
#include "WeakPointer.hpp"
#include "non_copyable.hpp"
#include "non_owing_pointer.hpp"

namespace plnt {
	class i_scene;

	namespace private_ {
		class TaskManagerConnection;
	}

	class i_game_object;

	class Task :
		public object, private util::non_copyable<Task> {
	public:
		using super = object;
		using GameObjectAccessorType = WeakPointer<i_game_object>;
		Task();
		virtual ~Task();
		virtual void Update() = 0;
		bool Pause();
		bool Resume();
		void Dispose();
		/*システム関数*/
		bool SystemSetUpAndInitialize(std::unique_ptr<private_::TaskManagerConnection> &&manager_connection,
		                              const WeakPointer<i_scene> &pscene);
		/*イベント*/
		/*! プロセス破棄イベント*/
		signal<void()> disposed;

	protected:
		//! シーンへのアクセス
		i_scene &scene() { return *scene_; }

	private:
		WeakPointer<i_scene> scene_;
		std::unique_ptr<private_::TaskManagerConnection> manager_connection_;
		virtual bool OnCreated() { return true; }

		virtual void OnDisposed() { };
	};

	PE_REFLECTABLE_CLASS(Task);
}

#define PE_TASK_CLASS(type)\
PE_REFLECTABLE_CLASS(type)
