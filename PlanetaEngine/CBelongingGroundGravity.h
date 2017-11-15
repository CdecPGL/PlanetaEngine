#pragma once
#include "GameObjectStandardComponent.h"

namespace planeta {
	class CTransform2D;
	/*! 地面方向への重力発生コンポーネント*/
	class CBelongingGroundGravity final: public GameObjectStandardComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CBelongingGroundGravity);
	public:
		using Super = GameObjectStandardComponent;
		/*! 重力スケールを取得*/
		const double gravity_scale()const { return _gravity_scale; }
		/*! 重力スケールを設定*/
		CBelongingGroundGravity& gravity_scale(double gs) { _gravity_scale = gs; return *this; }
	private:
		NonOwingPointer<CTransform2D> transform2d_;
		double _gravity_scale = 0.5;
		bool GetOtherComponentsProc(const GOComponentGetter& com_getter) override;
		void OnInitialized()override;
		void Update();
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CBelongingGroundGravity);
}