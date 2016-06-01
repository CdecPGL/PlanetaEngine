#include "CDrawLine2D.h"
#include "ScreenDrawer2D.h"
#include "IGameObjectForComponent.h"
#include "CTransform2D.h"
#include "Matrix2_2.h"

namespace planeta {
	CDrawLine2D::CDrawLine2D() :_wire_positions(2), _width(1), _length(0) {

	}

	void CDrawLine2D::DrawProc(ScreenDrawer2D& drawer) {
		//�g�����X�t�H�[�����烏�C���[�̈ʒu���X�V
		_wire_positions[0] = GetDrawCenterPosition();
		_wire_positions[1] = _wire_positions[0] + math::RotationalTransformation(GetDrawRotationRed(), Vector2Dd(1.0, 0.0))*length()*GetDrawScale().x;
		//�`��
		drawer.DrawWire(_wire_positions, _width*GetDrawScale().y, color());
	}
}