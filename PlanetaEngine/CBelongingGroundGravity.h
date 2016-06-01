#pragma once
#include "GameObjectStandardComponent.h"

namespace planeta {
	class CTransform2D;
	class CBelongingGroundGravity : public GameObjectStandardComponent {
	public:
		const double gravity_scale()const { return _gravity_scale; }
		CBelongingGroundGravity& gravity_scale(double gs) { _gravity_scale = gs; return *this; }
	private:
		util::NonOwingPointer<CTransform2D> transform2d_;
		double _gravity_scale = 0.5;
		bool OnInitialized() override;
		void Update();
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CBelongingGroundGravity);
}