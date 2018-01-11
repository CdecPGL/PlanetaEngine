﻿#pragma once

#include "planeta/core/Signal.hpp"
#include "planeta/core/Object.hpp"
#include "planeta/core/WeakPointer.hpp"
#include "NonCopyable.hpp"
#include "planeta/core/NonOwingPointer.hpp"

namespace plnt {
	class IScene;
	namespace private_ {
		class TaskManagerConnection;
	}
	class IGameObject;
	class Task :
		public Object, private util::NonCopyable<Task> {
	public:
		using Super = Object;
		using GameObjectAccessorType = WeakPointer<IGameObject>;
		Task();
		virtual ~Task();
		virtual void Update() = 0;
		bool Pause();
		bool Resume();
		void Dispose();
		/*システム関数*/
		bool SystemSetUpAndInitialize(std::unique_ptr<private_::TaskManagerConnection>&& manager_connection, const WeakPointer<IScene>& pscene);
		/*イベント*/
		/*! プロセス破棄イベント*/
		Signal<void()> disposed;
	protected:
		//! シーンへのアクセス
		IScene& scene() { return *scene_; }
	private:
		WeakPointer<IScene> scene_;
		std::unique_ptr<private_::TaskManagerConnection> manager_connection_;
		virtual bool OnCreated() { return true; }
		virtual void OnDisposed() {};
	};
	PE_REFLECTABLE_CLASS(Task);
}

#define PE_TASK_CLASS(type)\
PE_REFLECTABLE_CLASS(type)