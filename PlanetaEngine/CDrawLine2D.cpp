#include "CDrawLine2D.h"
#include "ScreenDrawer2D.h"
#include "IGameObject.h"
#include "CTransform2D.h"
#include "Matrix2_2.h"

namespace planeta {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDrawLine2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawLine2D, width)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawLine2D, length)
			.ShallowCopyTarget(&CDrawLine2D::_width)
			.ShallowCopyTarget(&CDrawLine2D::_length)
			.ShallowCopyTarget(&CDrawLine2D::_wire_positions);
	}

	CDrawLine2D::CDrawLine2D() :_wire_positions(2), _width(1), _length(0) {

	}

	void CDrawLine2D::DrawProc(ScreenDrawer2D& drawer) {
		//トランスフォームからワイヤーの位置を更新
		_wire_positions[0] = GetDrawCenterPosition();
		_wire_positions[1] = _wire_positions[0] + math::RotationalTransformation(GetDrawRotationRed(), Vector2Dd(1.0, 0.0))*length()*GetDrawScale().x;
		//描画
		drawer.DrawWire(_wire_positions, _width*GetDrawScale().y, color());
	}
}