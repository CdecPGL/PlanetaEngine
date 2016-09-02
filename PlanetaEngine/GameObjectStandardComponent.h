#pragma once

#include "GameObjectComponent.h"

namespace planeta {
	class GameObjectStandardComponent : public GameObjectComponent {
	public:
		using Super = GameObjectComponent;
		virtual ~GameObjectStandardComponent() = default;
	private:
		/*シーンデータのセットは行わない。子クラスにはオーバーライド不可にしておく。*/
		void SetSceneData(const util::WeakPointer<core::SceneData>&)override final{ return; }
	};
	PE_REFLECTABLE_CLASS(GameObjectStandardComponent);
}