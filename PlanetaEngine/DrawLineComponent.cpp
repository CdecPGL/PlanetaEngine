#include "DrawLineComponent.h"
#include "DrawManager.h"
#include "IGameObjectAccessor.h"
#include "TransformComponent.h"
#include "Matrix.h"

namespace planeta_engine {
	namespace components {
		DrawLineComponent::DrawLineComponent() :_wire_positions(2), _width(1), _length(0)
		{

		}

		void DrawLineComponent::Draw()
		{
			core::DrawManager& dm = core::DrawManager::instance();
			auto gt = game_object().GetTransformComponent();
			//�g�����X�t�H�[�����烏�C���[�̈ʒu���X�V
			_wire_positions[0] = GetDrawCenterPosition();
			_wire_positions[1] = _wire_positions[0] + math::RotationalTransformation(GetDrawRotationRed(), Vector2D<double>(1.0, 0.0))*length()*GetDrawScale().x;
			//�`��
			dm.DrawWire(_wire_positions, _width*GetDrawScale().y, color());
		}
	}
}