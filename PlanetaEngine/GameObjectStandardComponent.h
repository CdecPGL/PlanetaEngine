#pragma once

#include "GameObjectComponent.h"

namespace planeta_engine {
	class GameObjectStandardComponent : public GameObjectComponent {
	public:
		virtual ~GameObjectStandardComponent() = default;
	private:
		/*シーンデータのセットは行わない。子クラスにはオーバーライド不可にしておく。*/
		void SetSceneData(const utility::WeakPointer<core::SceneData>&)override final{ return; }
	};
}