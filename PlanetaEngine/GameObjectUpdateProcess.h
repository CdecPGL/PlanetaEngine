#pragma once
#include <memory>
#include <unordered_map>
#include "GameProcess.h"

namespace planeta_engine {
	namespace game {
		class GameObject;
	}
	namespace system_processes {
		class GameObjectUpdateProcess :
			public game::GameProcess
		{
		public:
			GameProcess::GameProcess;
			~GameObjectUpdateProcess();
			bool Resist(int id, const std::shared_ptr<game::GameObject>& game_object);
			bool Remove(int id);
		private:
			void Update()override final;
			std::unordered_map<int, std::shared_ptr<game::GameObject>> _game_objects;
			std::vector<int> remove_list_;
		};
	}
}
