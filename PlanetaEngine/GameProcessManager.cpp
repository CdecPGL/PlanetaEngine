#include "GameProcessManager.h"
#include "SceneAccessorForGameProcess.h"
#include "GameProcess.h"
#include "SceneData.h"
#include "SceneDataForGameProcess.h"
#include "GameProcessRegistrationData.h"
#include "GameProcessPosition.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace game {
		
		GameProcessManager::GameProcessManager(core::IGameAccessor& engine):game_(engine)
		{
		}

		GameProcessManager::~GameProcessManager() = default;

		void GameProcessManager::Update()
		{
			for (const auto& ps : process_priority_list_) {
				for (auto& p : ps) {
					p->Update();
				}
			}
		}

		bool GameProcessManager::Process()
		{
			//�o�^�������X�g�̃v���Z�X���폜����
			ProcessDisposeList_();
			return true;
		}

		bool GameProcessManager::RemoveGameProcess_(const InternalPosition& pos)
		{
			//�o�^�������X�g�ɒǉ�
			dispose_list_.push_back(pos);
			return true;
		}

		GameProcessManager::InternalPosition GameProcessManager::AddGameProcessToList_(const std::shared_ptr<GameProcess>& game_process, const core::GameProcessPosition& pos, InsertPosIndication ins_pos)
		{
			auto priority_list_it = CreateOrGetInsertPositionAtPriorityList_(pos, ins_pos);
			priority_list_it->push_back(game_process);
			auto list_it = --priority_list_it->end();

			return std::move(InternalPosition(priority_list_it, list_it));
		}

		void GameProcessManager::Finalize()
		{

			ProcessDisposeList_(); //�j�����X�g����
			//�܂����݂���v���Z�X�̏I���������s��
			for (auto& pl : process_priority_list_) {
				for (auto& p : pl) {
					p->Dispose();
				}
			}
			//�S���N���A
			dispose_list_.clear();
			process_name_id_map_.clear();
			process_id_map_.clear();
			process_priority_list_.clear();
		}

		void GameProcessManager::SetupProcess_(const std::shared_ptr<GameProcess>& game_process, std::function<bool()>&& remover, const core::GameProcessPosition& pos)
		{
			core::GameProcessRegistrationData registration_data;
			registration_data.scene_accessor = scene_accessor_;
			registration_data.disposer = std::move(remover);
			registration_data.position_in_list = pos;
			game_process->SystemSetUpAndInitialize(registration_data, *scene_data_);
		}

		void GameProcessManager::SetSceneInterface(core::ScenePublicInterface& spi)
		{
			scene_accessor_ = std::make_shared<core::SceneAccessorForGameProcess>(spi);
		}

		void GameProcessManager::SetSceneData(const core::SceneData& scene_data) {
			scene_data_ = std::make_unique<core::SceneDataForGameProcess>();
		}

		bool GameProcessManager::RegisterToIDMap_(GameProcess* id, const InternalPosition& pos_at_plist) {
			process_id_map_.emplace(id, pos_at_plist);
			return true;
		}

		void GameProcessManager::ProcessDisposeList_() {
			for (auto& pos : dispose_list_) {
				pos.iterator_at_priority_list->erase(pos.iterator_at_list);
				if (pos.iterator_at_priority_list->size() == 0) {
					process_priority_list_.erase(pos.iterator_at_priority_list);
				}
			}
			dispose_list_.clear();
		}

		bool GameProcessManager::RegisterProcessName_(const std::string& name, GameProcess* id) {
			process_name_id_map_.emplace(name, id);
			return true;
		}

		utility::WeakPointer<GameProcess> GameProcessManager::GetProcess(const std::string& name) {
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

		std::shared_ptr<GameProcess> GameProcessManager::CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPosition& pos, InsertPosIndication pos_indication) {
			auto np = creator(game_);
			auto new_pos = AddGameProcessToList_(np, pos, pos_indication);
			SetupProcess_(np, [new_pos, this]() {return RemoveGameProcess_(new_pos); }, core::GameProcessPosition(new_pos.iterator_at_priority_list));
			return RegisterToIDMap_(np.get(), new_pos) ? np : nullptr;
		}

		std::shared_ptr<GameProcess> GameProcessManager::CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPosition& pos, InsertPosIndication pos_indication, const std::string& name) {
			auto np = creator(game_);
			auto new_pos = AddGameProcessToList_(np, pos, pos_indication);
			SetupProcess_(np, [new_pos, this]() {return RemoveGameProcess_(new_pos); }, core::GameProcessPosition(new_pos.iterator_at_priority_list));
			return (RegisterToIDMap_(np.get(), new_pos) && RegisterProcessName_(name, np.get())) ? np : nullptr;
		}

		bool GameProcessManager::CreateKeyPosition(const std::string& id, const core::GameProcessPosition& pos, InsertPosIndication ins_ind) {
			auto key_pos_it = key_position_map_.find(id);
			if (key_pos_it != key_position_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, "���łɑ��݂���L�[�|�W�V����\"", id, "\"���č쐬���悤�Ƃ��܂����B");
				return false;
			}
			auto priority_list_it = CreateOrGetInsertPositionAtPriorityList_(pos, ins_ind);
			key_position_map_.emplace(std::make_pair(id, KeyPositionData(true, priority_list_it)));
			return true;
		}

		bool GameProcessManager::CreateSystemKeyPosition(const std::list<std::string>& key_position_ids) {
			key_position_map_.clear();
			for (const auto& key_position_id: key_position_ids) {
				process_priority_list_.push_back(core::GameProcessListType());
				key_position_map_.emplace(std::make_pair(key_position_id, KeyPositionData(false, --process_priority_list_.end())));
			}
			return true;
			
		}

		core::GameProcessPriorytyListType::iterator GameProcessManager::CreateOrGetInsertPositionAtPriorityList_(const core::GameProcessPosition& pos, InsertPosIndication ins_pos) {
			auto priority_list_it = pos.priority_list_iterator;
			switch (ins_pos) {
			case planeta_engine::InsertPosIndication::At:
				break;
			case planeta_engine::InsertPosIndication::Before:
				priority_list_it = process_priority_list_.insert(priority_list_it, core::GameProcessListType()); //���O�ɑ}��
				break;
			case planeta_engine::InsertPosIndication::After:
				++priority_list_it;
				if (priority_list_it == process_priority_list_.end()) { //�����Ōゾ�����疖���ɒǉ�
					process_priority_list_.push_back(core::GameProcessListType());
					priority_list_it = --process_priority_list_.end();
				} else { //�Ō�o�Ȃ������玟�̗v�f�̒��O�ɑ}��
					priority_list_it = process_priority_list_.insert(priority_list_it, core::GameProcessListType());
				}
				break;
			default:
				break;
			}
			return priority_list_it;
		}

		bool GameProcessManager::DeleteKeyPosition(const std::string& id) {
			auto key_pos_it = key_position_map_.find(id);
			if (key_pos_it == key_position_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "�L�[�|�W�V����\"", id, "\"�����݂��Ȃ����ߍ폜�ł��܂���ł����B");
				return false;
			}
			if (key_pos_it->second.deletable == false) {
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, "�폜�ł��Ȃ��L�[�|�W�V����\"", id, "\"���폜���悤�Ƃ��܂����B");
				return false;
			}
			//�폜�Ώۂ̃��X�g���󂾂�����폜����
			if (key_pos_it->second.position.priority_list_iterator->size() == 0) {
				process_priority_list_.erase(key_pos_it->second.position.priority_list_iterator);
			}
			key_position_map_.erase(key_pos_it);
			return true;
		}

		core::GameProcessPosition GameProcessManager::GetKeyPosition(const std::string& id) const {
			auto key_pos_it = key_position_map_.find(id);
			if (key_pos_it == key_position_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "�L�[�|�W�V����\"", id, "\"�͑��݂��܂���B");
				throw std::out_of_range("key position doesnt exist.");
			}
			return key_pos_it->second.position;
		}
	}
}