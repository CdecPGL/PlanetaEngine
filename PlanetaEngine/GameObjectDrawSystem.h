#pragma once

#include <unordered_map>
#include <map>
#include <list>
#include <memory>
#include "NonCopyable.h"
#include "SceneModule.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	class ScreenDrawerGUI;
	class CDraw2D;
	namespace core {
		class GameObjectDrawSystem :public core::SceneModule, private utility::NonCopyable<GameObjectDrawSystem>
		{
		public:
			GameObjectDrawSystem();
			bool Initialize()override;
			void Finalize()override;
			void Update()override;
			void ExcuteDraw();
			/*�`��R���|�[�l���g�o�^*/
			void Register(const std::shared_ptr<CDraw2D>& draw_component, int priority) {
				_draw_component_update_list[priority].push_back(draw_component);
				_draw_component_map.emplace(draw_component.get(), std::make_pair(priority, --_draw_component_update_list[priority].end()));
			}
			/*�`��R���|�[�l���g�o�^����*/
			bool Remove(const std::shared_ptr<CDraw2D>& draw_component);
			/*�`��D��x�ύX*/
			bool ChangePriority(const std::shared_ptr<CDraw2D>& draw_component, int priority) {
				if (!Remove(draw_component)) { return false; }
				Register(draw_component, priority);
				return true;
			}
		private:
			/*�X�V���X�g(map��list�͗v�f�̒ǉ��폜���s���Ă��C�e���[�^���L��)*/
			std::map<int, std::list<std::shared_ptr<CDraw2D>>> _draw_component_update_list;
			/*�`��R���|�[�l���g�}�b�v<�|�C���^�A�X�V���X�g�ւ̃C�e���[�^>*/
			std::unordered_map<CDraw2D*, std::pair<int, std::list<std::shared_ptr<CDraw2D>>::iterator>> _draw_component_map;
			std::unique_ptr<ScreenDrawer2D> screen_drawer_2d_;
			std::unique_ptr<ScreenDrawerGUI> screen_drawer_gui_;
		};
	}
}
