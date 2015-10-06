#include "GameObjectUpdateProcess.h"
#include "GameObject.h"
#include "SystemLog.h"

namespace planeta_engine{
	namespace system_processes {


		GameObjectUpdateProcess::~GameObjectUpdateProcess()
		{
		}

		bool GameObjectUpdateProcess::Resist(int id, const std::shared_ptr<game::GameObject>& game_object)
		{
			if (_game_objects.find(id) != _game_objects.end()) { return false; }
			else {
				_game_objects.insert(std::make_pair(id, game_object));
				return true;
			}
		}

		bool GameObjectUpdateProcess::Remove(int id)
		{
			auto it = _game_objects.find(id);
			if (it == _game_objects.end()) { return false; }
			else {
				remove_list_.push_back(id);
				return true;
			}
		}

		void GameObjectUpdateProcess::Update()
		{
			//�폜���X�g�̏���
			for (int id : remove_list_) {
				auto it = _game_objects.find(id);
				if (it != _game_objects.end()) { _game_objects.erase(it); }
			}
			//�X�V����
			for (auto it = _game_objects.begin(); it != _game_objects.end(); ++it) {
				try {
					it->second->Update();
				}
				catch (utility::NullWeakPointerException& e) {
					debug::SystemLog::instance().LogError(std::string("GameObject::Update�Ŗ�����WeakPointer���Q�Ƃ���܂����B���̔�������GameObject�̓��X�g���珜�O����܂��B") + e.what(), "GameObjectUpdateProcess::Update");
					_game_objects.erase(it);
					break;
				}
			}
		}

	}
}
