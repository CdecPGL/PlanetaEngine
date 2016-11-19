#pragma once

#include "IDrawSystem.h"
#include "SceneModule.h"

namespace planeta {
	class CDraw2D;
	class CCamera2D;
	namespace private_{
		class DrawSystem : public IDrawSystem, public SceneModule {
		public:
			virtual ~DrawSystem() = 0 {};
			virtual void ExcuteDraw() = 0;
			virtual void ApplyCameraState() = 0;
			/*描画コンポーネント登録*/
			virtual void Register(const std::shared_ptr<CDraw2D>& draw_component, int priority) = 0;
			/*描画コンポーネント登録解除*/
			virtual bool Remove(const std::shared_ptr<CDraw2D>& draw_component) = 0;
			/*描画優先度変更*/
			virtual bool ChangePriority(const std::shared_ptr<CDraw2D>& draw_component, int priority) = 0;
			/*カメラコンポーネント登録*/
			virtual bool RegisterCamera(const std::shared_ptr<CCamera2D>& camera_component) = 0;
			/*カメラコンポーネント登録解除*/
			virtual void RemoveCamera(CCamera2D* camera_component) = 0;
		};
	}
}