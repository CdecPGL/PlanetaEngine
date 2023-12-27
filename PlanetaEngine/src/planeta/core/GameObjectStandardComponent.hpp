#pragma once

#include "GameObjectComponent.hpp"

namespace plnt {
	class GameObjectStandardComponent : public GameObjectComponent {
	public:
		using super = GameObjectComponent;
		virtual ~GameObjectStandardComponent() = default;

	private:
		/*シーンデータのセットは行わない。子クラスにはオーバーライド不可にしておく。*/
		void SetSceneInternalInterface(const WeakPointer<private_::ISceneInternal> &) override final { return; }
	};

	PE_REFLECTABLE_CLASS(GameObjectStandardComponent);
}
