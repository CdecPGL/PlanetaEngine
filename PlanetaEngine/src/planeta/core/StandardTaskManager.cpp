#include <unordered_map>
#include <list>
#include <array>
#include <type_traits>
#include <bitset>

#include "StandardTaskManager.hpp"
#include "task.hpp"
#include "task_manager_connection.hpp"
#include "log_utility.hpp"
#include "system_task_slot.hpp"
#include "i_debug_manager.hpp"
#include "i_scene_internal.hpp"

namespace plnt::private_ {
	namespace {
		constexpr int task_slot_0 = 0;
		constexpr int task_slot_1 = 1;
		constexpr int task_slot_2 = 2;
		constexpr int apply_velocity_slot = 3;
		constexpr int collision_detection_slot = 4;
		constexpr int task_slot_3 = 5;
		constexpr int task_slot_4 = 6;
		constexpr int task_slot_5 = 7;
		constexpr int task_slot_6 = 8;
		constexpr int apply_camera_slot = 9;
		constexpr int task_slot_7 = 10;
		constexpr int task_slot_8 = 11;
		constexpr int task_slot_9 = 12;
		constexpr int task_slot_10 = 13;
		constexpr int draw_slot = 14;
		constexpr int draw_gui_slot = 15;
		constexpr int slot_count = 16;
		//スロットマップ
		constexpr std::array slot_group_number_map = {
			task_slot_0,
			task_slot_1,
			task_slot_2,
			task_slot_3,
			task_slot_4,
			task_slot_5,
			task_slot_6,
			task_slot_7,
			task_slot_8,
			task_slot_9,
			task_slot_10,
		};
		//システムスロットマップ
		using namespace private_;
		constexpr std::array system_slot_group_number_map = {
			apply_velocity_slot,
			collision_detection_slot,
			apply_camera_slot,
			draw_slot,
			draw_gui_slot,
		};
		/*タスクスロットからタスクグループ番号を取得*/
		int get_group_number_from_slot(task_slot slot) {
			return slot_group_number_map[static_cast<std::underlying_type_t<task_slot>>(slot)];
		}

		/*システムタスクスロットからタスクグループ番号を取得*/
		int get_group_number_from_system_slot(system_task_slot slot) {
			return system_slot_group_number_map[static_cast<std::underlying_type_t<system_task_slot>>(
				slot)];
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class standard_task_manager::impl {
	public:
		struct task_data;
		//タスク群タイプ
		using task_group_type = std::list<task *>;
		//タスクリストタイプ
		using task_list_type = std::list<std::shared_ptr<task_data>>;
		//名前マップタイプ
		using name_map_type = std::unordered_map<std::string, std::shared_ptr<task_data>>;

		/*タスクの状態(順に、実行中、停止中、破棄された)*/
		enum class task_state { running, pausing, disposed, pause_requested, resume_requested, dispose_requested };

		/*タスクの情報*/
		// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
		struct task_data {
			task_data() = default;
			task_data(const task_data &) = delete;
			task_data &operator=(const task_data &) = delete;
			std::shared_ptr<task> task;
			task_list_type::iterator iterator_at_task_data_list;
			name_map_type::iterator iterator_at_task_name_map;
			bool is_named = false;
			int group_number; //所属しているタスクグループ番号
			task_group_type::iterator iterator_at_task_execution_list; //所属しているタスクグループでのイテレータ
			task_state state = task_state::pausing;
		};

	private:
		//タスク群リスト
		std::array<task_group_type, slot_count> task_execution_list_;

		/*タスクリスト*/
		task_list_type task_data_list_;
		/*名前マップ。<タスク名,タスクID>*/
		name_map_type task_name_map_;
		/*管理処理リスト*/
		std::vector<std::function<void()>> management_process_list_;
		/**/
		bool is_system_task_disposable_ = false; //システムタスクが削除可能か。
	public:
		impl() = default;

		//システムタスク削除の有効化
		void validate_system_task_disposal() { is_system_task_disposable_ = true; }
		//////////////////////////////////////////////////////////////////////////
		/*タスク削除要請*/
		bool dispose_task_request(task_data &t_data, const bool is_system_task) {
			//実行中、停止中、実行再開依頼中、停止依頼中に有効
			if (t_data.state == task_state::disposed ||
				t_data.state == task_state::dispose_requested) { return false; }
			//システムタスクの削除をチェック
			if (is_system_task == true && is_system_task_disposable_ == false) {
				PE_LOG_ERROR("システムタスクの削除は許可されていません。");
			}
			//削除処理を追加
			switch (t_data.state) {
				case task_state::running:
				case task_state::resume_requested:
					management_process_list_.emplace_back([&t_data, this] {
						inactivate_task(t_data);
						dispose_task(t_data);
					});
					break;
				case task_state::pausing:
				case task_state::pause_requested:
					management_process_list_.emplace_back([&t_data, this] { dispose_task(t_data); });
					break;
				case task_state::disposed:
				case task_state::dispose_requested:
					assert(false);
					return false;
			}
			t_data.state = task_state::dispose_requested;
			return true;
		}

		/*タスク停止要請*/
		bool pause_task_request(task_data &t_data) {
			//実行中か、実行再開依頼が発行されている場合にのみ有効
			if (t_data.state == task_state::disposed ||
				t_data.state == task_state::dispose_requested ||
				t_data.state == task_state::pausing ||
				t_data.state == task_state::pause_requested
			) { return false; }
			//停止処理を追加
			management_process_list_.emplace_back([&t_data, this] { inactivate_task(t_data); });
			t_data.state = task_state::pause_requested;
			return true;
		}

		/*タスク再開要請*/
		bool resume_task_request(task_data &t_data) {
			//停止か、停止依頼が発行されている場合にのみ有効
			if (t_data.state == task_state::disposed ||
				t_data.state == task_state::dispose_requested ||
				t_data.state == task_state::running ||
				t_data.state == task_state::resume_requested
			) { return false; }
			//再開処理を追加
			/*management_process_list_.push_back([&t_data, this] {
				ActivateTask(t_data);
				});*/
			t_data.state = task_state::resume_requested;
			//再開処理はすぐに実行する。(実行リストにはstd::listを用いており、ループ中に末尾に追加されても問題ないため)
			activate_task(t_data);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		/*タスクの有効化*/
		void activate_task(task_data &t_data) {
			task_execution_list_[t_data.group_number].push_back(t_data.task.get());
			t_data.iterator_at_task_execution_list = --task_execution_list_[t_data.group_number].end();
			t_data.state = task_state::running;
		}

		/*タスクの無効化*/
		bool inactivate_task(task_data &t_data) {
			task_execution_list_[t_data.group_number].erase(t_data.iterator_at_task_execution_list);
			t_data.state = task_state::pausing;
			return true;
		}

		/*タスクの破棄*/
		bool dispose_task(task_data &t_data) {
			task_data_list_.erase(t_data.iterator_at_task_data_list); //タスクリストから削除
			if (t_data.is_named) {
				task_name_map_.erase(t_data.iterator_at_task_name_map); //名前マップから削除
			}
			if (t_data.state == task_state::running) {
				task_execution_list_[t_data.group_number].erase(t_data.iterator_at_task_execution_list);
				//タスクグループリストから削除
			}
			t_data.state = task_state::disposed;
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		/*タスクの設定*/
		void setup_task(const std::shared_ptr<task_data> &t_data, const bool is_system_task,
		                const weak_pointer<i_scene> &i_scene) {
			//ここでラムダ関数がt_dataのシェアポをキャプチャしておくことで、t_data使用中の解放を防ぐ。
			auto manager_connection = std::make_unique<
				task_manager_connection>(
				[this, t_data] { return pause_task_request(*t_data); }, //Pauser
				[this, t_data] { return resume_task_request(*t_data); }, //Resumer
				is_system_task
					? std::function([this, t_data] { dispose_task_request(*t_data, true); })
					: [this, t_data] { dispose_task_request(*t_data, false); } //Disposer(システムタスクの場合は削除できないDisposerをセット)
			);
			t_data->task->system_set_up_and_initialize(std::move(manager_connection), i_scene);
		}

		/*タスクをマップに登録*/
		std::shared_ptr<task_data> register_task_to_list(const std::shared_ptr<task> &task, const int group_number,
		                                                 const bool auto_run) {
			auto pt_data = std::make_shared<task_data>();
			pt_data->task = task;
			pt_data->group_number = group_number;
			pt_data->is_named = false;
			pt_data->state = task_state::pausing;
			//タスクデータリストに登録して、自身を指すイテレータを保持
			task_data_list_.push_back(pt_data);
			pt_data->iterator_at_task_data_list = --task_data_list_.end();
			//タスクグループリストに登録して、自身を指すイテレータを保持
			if (auto_run) {
				pt_data->state = task_state::running;
				task_execution_list_[group_number].push_back(task.get());
				pt_data->iterator_at_task_execution_list = --task_execution_list_[group_number].end();
			}

			return pt_data;
		}

		/*名前を登録*/
		bool register_task_name(const std::string &name, const std::shared_ptr<task_data> &pt_data) {
			//名前マップに登録し、タスクデータにそのイテレータをセットする。
			pt_data->iterator_at_task_name_map = task_name_map_.emplace(name, pt_data).first;
			pt_data->is_named = true;
			return true;
		}

		/*管理処理Queを処理する*/
		void handle_management_que() {
			for (auto &prc : management_process_list_) { prc(); }
			management_process_list_.clear();
		}

		std::shared_ptr<task> get_task(const std::string &name) {
			const auto nit = task_name_map_.find(name);
			return nit == task_name_map_.end() ? nullptr : nit->second->task;
		}

		//有効なタスクのメンバ関数を実行
		void execute_valid_tasks_function(void (task::*func)()) const {
			for (int i = 0; i < slot_count; ++i) {
				for (auto &tg = task_execution_list_[i]; const auto &t : tg) { (t->*func)(); }
			}
		}

		//全削除
		void all_clear() {
			management_process_list_.clear();
			task_name_map_.clear();
			task_data_list_.clear();
			for (auto &&tg : task_execution_list_) { tg.clear(); }
		}

		//タスク数を数える
		void count_task(int &active_task_count, int &inactive_task_count) const {
			active_task_count = 0;
			inactive_task_count = 0;
			for (auto &&td : task_data_list_) {
				switch (td->state) {
					case task_state::running:
					case task_state::pause_requested:
						++active_task_count;
						break;
					case task_state::pausing:
					case task_state::resume_requested:
						++inactive_task_count;
						break;
					case task_state::disposed:
					case task_state::dispose_requested:
						break;
				}
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//TaskManager
	//////////////////////////////////////////////////////////////////////////
	standard_task_manager::standard_task_manager() : impl_(std::make_unique<impl>()) {}

	standard_task_manager::~standard_task_manager() = default;

	void standard_task_manager::execute_task() { impl_->execute_valid_tasks_function(&task::update); }

	void standard_task_manager::update() {
		//管理処理を行う
		impl_->handle_management_que();
	}

	void standard_task_manager::finalize() {
		impl_->handle_management_que(); //管理処理
		impl_->validate_system_task_disposal(); //システムタスクを削除可能に。
		//まだ存在するプロセスの終了処理を行う
		impl_->execute_valid_tasks_function(&task::dispose);
		//全部クリア
		impl_->all_clear();
	}

	weak_pointer<task> standard_task_manager::get_task(const std::string &name) const { return impl_->get_task(name); }

	bool standard_task_manager::register_task(const std::shared_ptr<task> &task, const task_slot slot,
	                                        const bool auto_run) {
		const int group_number = get_group_number_from_slot(slot);
		const auto pt_data = impl_->register_task_to_list(task, group_number, auto_run);
		impl_->setup_task(pt_data, false, scene_internal_interface());
		return task != nullptr;
	}

	bool standard_task_manager::register_task(const std::shared_ptr<task> &task, const task_slot slot,
	                                        const std::string &name, const bool auto_run) {
		const int group_number = get_group_number_from_slot(slot);
		const auto pt_data = impl_->register_task_to_list(task, group_number, auto_run);
		impl_->setup_task(pt_data, false, scene_internal_interface());
		return impl_->register_task_name(name, pt_data);
	}

	std::shared_ptr<task> standard_task_manager::register_system_task(const std::shared_ptr<task> &task,
	                                                                const system_task_slot slot) {
		const int group_number = get_group_number_from_system_slot(slot);
		const auto pt_data = impl_->register_task_to_list(task, group_number, true);
		impl_->setup_task(pt_data, true, scene_internal_interface());
		return task;
	}

	void standard_task_manager::debug_information_add_handle(i_debug_information_adder &di_adder) {
		di_adder.add_line("-----TaskManager-----");
		int active_task{0}, inactive_task{0};
		impl_->count_task(active_task, inactive_task);
		di_adder.add_line_v("実行中タスク数:", active_task);
		di_adder.add_line_v("停止中タスク数:", inactive_task);
	}
}
