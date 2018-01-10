#pragma once

#include "planeta/IDrawSystem.hpp"
#include "SceneModule.hpp"

namespace plnt {
	class CDraw2D;
	class CDrawGUI;
	class CCamera2D;
	namespace private_{
		class CDraw2DManagerConnection {
		public:
			virtual ~CDraw2DManagerConnection() = 0 {};
			virtual bool Activte() = 0;
			virtual bool Inactivate() = 0;
			virtual bool Remove() = 0;
			virtual bool ChangePriority(int priority) = 0;
		};
		class CDrawGUIManagerConnection {
		public:
			virtual ~CDrawGUIManagerConnection() = 0 {};
			virtual bool Activte() = 0;
			virtual bool Inactivate() = 0;
			virtual bool Remove() = 0;
			virtual bool ChangePriority(int priority) = 0;
		};
		class CCamera2DManagerConnection {
		public:
			virtual ~CCamera2DManagerConnection() = 0 {};
			virtual bool Remove() = 0;
		};

		class DrawSystem : public IDrawSystem, public SceneModule {
		public:
			virtual ~DrawSystem() = 0 {};
			/*�Q�[���I�u�W�F�N�g�̕`��*/
			virtual void ExcuteDraw() = 0;
			/*GUI�̕`��*/
			virtual void ExcuteDrawGUI() = 0;
			/*�J������Ԃ̓K�p*/
			virtual void ApplyCameraState() = 0;
			/*2D�`��R���|�[�l���g�o�^*/
			virtual std::unique_ptr<CDraw2DManagerConnection> RegisterCDraw2D(const std::shared_ptr<CDraw2D>& draw_component, int priority) = 0;
			/*GUI�`��R���|�[�l���g�o�^*/
			virtual std::unique_ptr<CDrawGUIManagerConnection> RegisterCDrawGUI(const std::shared_ptr<CDrawGUI>& draw_component, int priority) = 0;
			/*�J�����R���|�[�l���g�o�^*/
			virtual std::unique_ptr<CCamera2DManagerConnection> RegisterCCamera2D(const std::shared_ptr<CCamera2D>& camera_component) = 0;
		};
	}
}