#pragma once
#include "GameObjectStandardComponent.h"

namespace planeta {
	class CTransform2D;
	class CBelongingGroundGravity final: public GameObjectStandardComponent {
	public:
		using Super = GameObjectStandardComponent;
		const double gravity_scale()const { return _gravity_scale; }
		CBelongingGroundGravity& gravity_scale(double gs) { _gravity_scale = gs; return *this; }
	private:
		util::NonOwingPointer<CTransform2D> transform2d_;
		double _gravity_scale = 0.5;
		bool OnInitialized(const GOComponentGetter& com_getter) override;
		void Update();
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CBelongingGroundGravity);
}