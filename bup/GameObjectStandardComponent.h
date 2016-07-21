#pragma once

#include "GameObjectComponent.h"

namespace planeta {
	class GameObjectStandardComponent : public GameObjectComponent {
	public:
		virtual ~GameObjectStandardComponent() = default;
	private:
		/*シーンデータのセットは行わない。子クラスにはオーバーライド不可にしておく。*/
		void SetSceneData(const util::WeakPointer<core::SceneData>&)override final{ return; }
	};
}