#pragma once

#include "GameObjectComponent.h"

namespace planeta {
	class GameObjectStandardComponent : public GameObjectComponent {
	public:
		using Super = GameObjectComponent;
		virtual ~GameObjectStandardComponent() = default;
	private:
		/*シーンデータのセットは行わない。子クラスにはオーバーライド不可にしておく。*/
		void SetSceneInternalInterface(const WeakPointer<private_::ISceneInternal>&)override final{ return; }
	};
	PE_REFLECTABLE_CLASS(GameObjectStandardComponent);
}