#pragma once
#include "GameProcess.h"
#include <unordered_map>
#include <map>
#include <list>
#include <memory>

namespace planeta_engine {
	namespace components {
		class DrawComponent;
	}
	namespace system_processes {
		class GameObjectDrawProcess :
			public game::GameProcess
		{
		public:
			using GameProcess::GameProcess;
			~GameObjectDrawProcess();
			/*�`��R���|�[�l���g�o�^*/
			void Resist(const std::shared_ptr<components::DrawComponent>& draw_component, int priority) {
				_draw_component_update_list[priority].push_back(draw_component);
				_draw_component_map.emplace(draw_component.get(), std::make_pair(priority, --_draw_component_update_list[priority].end()));
			}
			/*�`��R���|�[�l���g�o�^����*/
			bool Remove(const std::shared_ptr<components::DrawComponent>& draw_component);
			/*�`��D��x�ύX*/
			bool ChangePriority(const std::shared_ptr<components::DrawComponent>& draw_component, int priority) {
				if (!Remove(draw_component)) { return false; }
				Resist(draw_component, priority);
				return true;
			}
		private:
			void Update()override final;
			/*�X�V���X�g(map��list�͗v�f�̒ǉ��폜���s���Ă��C�e���[�^���L��)*/
			std::map<int, std::list<std::shared_ptr<components::DrawComponent>>> _draw_component_update_list;
			/*�`��R���|�[�l���g�}�b�v<�|�C���^�A�X�V���X�g�ւ̃C�e���[�^>*/
			std::unordered_map<components::DrawComponent*, std::pair<int, std::list<std::shared_ptr<components::DrawComponent>>::iterator>> _draw_component_map;
		};
	}
}
