#pragma once

#include "signal.hpp"
#include "object.hpp"
#include "weak_pointer.hpp"
#include "non_copyable.hpp"

namespace plnt {
	class i_scene;

	namespace private_ {
		class task_manager_connection;
	}

	class i_game_object;

	class task :
		public object, util::non_copyable<task> {
	public:
		using super = object;
		using game_object_accessor_type = weak_pointer<i_game_object>;

		task();
		~task() override;

		virtual void update() = 0;
		bool pause() const;
		bool resume() const;
		void dispose();
		/*システム関数*/
		bool system_set_up_and_initialize(std::unique_ptr<private_::task_manager_connection> &&manager_connection,
		                                  const weak_pointer<i_scene> &p_scene);
		/*イベント*/
		/*! プロセス破棄イベント*/
		signal<void()> disposed;

	protected:
		//! シーンへのアクセス
		[[nodiscard]] i_scene &scene() const { return *scene_; }

	private:
		weak_pointer<i_scene> scene_;
		std::unique_ptr<private_::task_manager_connection> manager_connection_;
		virtual bool on_created() { return true; }

		virtual void on_disposed() {}
	};

	PE_REFLECTABLE_CLASS(task);
}

#define PE_TASK_CLASS(type)\
PE_REFLECTABLE_CLASS(type)
