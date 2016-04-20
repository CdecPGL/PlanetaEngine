#include "TaskManager.h"
#include "SceneAccessorForTask.h"
#include "Task.h"
#include "SceneData.h"
#include "SceneDataForTask.h"
#include "TaskRegistrationData.h"
#include "TaskPosition.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace game {
		
		TaskManager::TaskManager(core::IGameAccessor& engine):game_(engine)
		{
		}

		TaskManager::~TaskManager() = default;

		void TaskManager::Update()
		{
			for (const auto& ps : process_priority_list_) {
				for (auto& p : ps) {
					p->Update();
				}
			}
		}

		bool TaskManager::Process()
		{
			//登録解除リストのプロセスを削除する
			ProcessDisposeList_();
			return true;
		}

		bool TaskManager::RemoveTask_(const InternalPosition& pos)
		{
			//登録解除リストに追加
			dispose_list_.push_back(pos);
			return true;
		}

		TaskManager::InternalPosition TaskManager::AddTaskToList_(const std::shared_ptr<Task>& game_process, const core::TaskPosition& pos)
		{
			auto priority_list_it = CreateOrGetInsertPositionAtPriorityList_(pos);
			priority_list_it->push_back(game_process);
			auto list_it = --priority_list_it->end();

			return std::move(InternalPosition(priority_list_it, list_it));
		}

		void TaskManager::Finalize()
		{

			ProcessDisposeList_(); //破棄リスト処理
			//まだ存在するプロセスの終了処理を行う
			for (auto& pl : process_priority_list_) {
				for (auto& p : pl) {
					p->Dispose();
				}
			}
			//全部クリア
			dispose_list_.clear();
			process_name_id_map_.clear();
			process_id_map_.clear();
			process_priority_list_.clear();
		}

		void TaskManager::SetupProcess_(const std::shared_ptr<Task>& game_process, std::function<bool()>&& remover, const core::TaskPosition& pos)
		{
			core::TaskRegistrationData registration_data;
			registration_data.scene_accessor = scene_accessor_;
			registration_data.disposer = std::move(remover);
			registration_data.position_in_list = pos;
			game_process->SystemSetUpAndInitialize(registration_data, *scene_data_);
		}

		void TaskManager::SetSceneInterface(core::ScenePublicInterface& spi)
		{
			scene_accessor_ = std::make_shared<SceneAccessorForTask>(spi);
		}

		void TaskManager::SetSceneData(const core::SceneData& scene_data) {
			scene_data_ = std::make_unique<core::SceneDataForTask>();
		}

		bool TaskManager::RegisterToIDMap_(Task* id, const InternalPosition& pos_at_plist) {
			process_id_map_.emplace(id, pos_at_plist);
			return true;
		}

		void TaskManager::ProcessDisposeList_() {
			for (auto& pos : dispose_list_) {
				pos.iterator_at_priority_list->erase(pos.iterator_at_list);
				if (pos.iterator_at_priority_list->size() == 0) {
					process_priority_list_.erase(pos.iterator_at_priority_list);
				}
			}
			dispose_list_.clear();
		}

		bool TaskManager::RegisterProcessName_(const std::string& name, Task* id) {
			process_name_id_map_.emplace(name, id);
			return true;
		}

		utility::WeakPointer<Task> TaskManager::GetProcess(const std::string& name) {
			auto nit = process_name_id_map_.find(name);
			if (nit == process_name_id_map_.end()) { return nullptr; }
			auto pit = process_id_map_.find(nit->second);
			if (pit == process_id_map_.end()) {
				process_name_id_map_.erase(nit);
				return nullptr;
			} else {
				return *pit->second.iterator_at_list;
			}
		}

		std::shared_ptr<Task> TaskManager::CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, const core::TaskPosition& pos) {
			auto np = creator(game_);
			auto new_pos = AddTaskToList_(np, pos);
			SetupProcess_(np, [new_pos, this]() {return RemoveTask_(new_pos); }, core::TaskPosition(new_pos.iterator_at_priority_list));
			return RegisterToIDMap_(np.get(), new_pos) ? np : nullptr;
		}

		std::shared_ptr<Task> TaskManager::CreateTask(const std::function<std::shared_ptr<Task>(core::IGameAccessor&)>& creator, const core::TaskPosition& pos, const std::string& name) {
			auto np = creator(game_);
			auto new_pos = AddTaskToList_(np, pos);
			SetupProcess_(np, [new_pos, this]() {return RemoveTask_(new_pos); }, core::TaskPosition(new_pos.iterator_at_priority_list));
			return (RegisterToIDMap_(np.get(), new_pos) && RegisterProcessName_(name, np.get())) ? np : nullptr;
		}

		bool TaskManager::SetKeyPositions(const std::list<std::string>& key_position_ids) {
			key_position_map_.clear();
			for (const auto& key_position_id: key_position_ids) {
				process_priority_list_.push_back(core::TaskListType());
				key_position_map_.emplace(std::make_pair(key_position_id, core::TaskPosition(--process_priority_list_.end())));
			}
			return true;
			
		}

		core::TaskPriorytyListType::iterator TaskManager::CreateOrGetInsertPositionAtPriorityList_(const core::TaskPosition& pos) {
			auto priority_list_it = pos.priority_list_iterator;
			return priority_list_it;
		}

		core::TaskPosition TaskManager::GetKeyPosition(const std::string& id) const {
			auto key_pos_it = key_position_map_.find(id);
			if (key_pos_it == key_position_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "キーポジション\"", id, "\"は存在しません。");
				throw std::out_of_range("key position doesn't exist.");
			}
			return core::TaskPosition(key_pos_it->second.priority_list_iterator);
		}
	}
}