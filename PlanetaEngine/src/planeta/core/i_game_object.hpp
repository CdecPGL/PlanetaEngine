#pragma once

#include <type_traits>
#include <memory>

#include "Signal.hpp"
#include "ITaskManager.hpp"
#include "TaskSlot.hpp"
#include "IScene.hpp"

namespace plnt {
	class Task;
	class game_object_component;
	class Task;

	/*! ゲームオブジェクトの状態*/
	enum class game_object_state { invalid, inactive, active, initializing, inactivating, activating };

	/*! @brief ゲームオブジェクトへのアクセスを提供するインターフェイスクラス
	*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_game_object {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_game_object() = 0 {}

		//! ゲームオブジェクトをシーン内で有効化する。戻り値は有効化の成否を示す。  
		virtual void activate() = 0;
		//! ゲームオブジェクトをシーン内で無効化する。戻り値は無効化の成否を示す。  
		virtual void inactivate() = 0;
		//! ゲームオブジェクトをシーンから破棄する
		virtual void dispose() = 0;
		//! 自身のWeakPointerを取得する
		[[nodiscard]] virtual WeakPointer<i_game_object> get_pointer() = 0;
		//! コンポーネントを型で取得する
		template <class ComT>
		[[nodiscard]] WeakPointer<ComT> get_component() const {
			static_assert(std::is_base_of_v<game_object_component, ComT> == true,
			              "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(get_component_by_type_info(
				typeid(ComT), [](game_object_component *goc) {
					return dynamic_cast<ComT *>(goc) !=
						nullptr;
				}));
		}

		//! 特定型のコンポーネントを持っているか確認する
		// TODO

		//! コンポーネントを型ですべて取得する
		// TODO

		//! コンポーネントをコンポーネント型IDで取得する
		// TODO

		//! コンポーネントをコンポーネント型IDで全て取得する
		// TODO

		//! 特定のコンポーネント型IDのコンポーネントを持っているか確認
		// TODO

		/*! @brief タスクをアタッチする

			テンプレート引数で指定した型のタスクを作成し、アタッチする。
			ゲームオブジェクトのアタッチされたタスクの寿命はそのゲームオブジェクトと同じになり、ゲームオブジェクトの無効化有効化に合わせて停止、再開する。
		*/
		template <class T>
		[[nodiscard]] WeakPointer<T> create_and_attach_task(const TaskSlot slot) {
			static_assert(std::is_base_of_v<Task, T> == true, "T must derive Task");
			auto task = scene().task_manager().CreateTask<T>(slot);
			if (task == nullptr) { return nullptr; }
			set_up_attached_task(task);
			return task;
		}

		//! ゲームオブジェクトの状態を取得する
		[[nodiscard]] virtual game_object_state state() const = 0;
		//! シーンへのアクセスを取得する
		[[nodiscard]] virtual IScene &scene() = 0;

		/*イベント*/
		/*! 有効化イベント*/
		Signal<void()> activated;
		/*! 無効化イベント*/
		Signal<void()> inactivated;
		/*! 破棄イベント*/
		Signal<void()> disposed;

	protected:
		[[nodiscard]] virtual std::shared_ptr<game_object_component> get_component_by_type_info(
			const std::type_info &ti, const std::function<bool(game_object_component *goc)> &type_checker) const = 0;
		virtual void set_up_attached_task(const WeakPointer<Task> &task) = 0;
	};
}
