#pragma once

#include <vector>
#include <array>
#include "CDraw2D.h"

namespace planeta {
	class CDrawPolygon2D final : public CDraw2D {
	public:
		void SetNumberOfVertexes(size_t n);
		/*�w��C���f�b�N�X�̍��W��ݒ�*/
		bool SetVertexPosition(size_t idx, const Vector2Dd& pos);
		/*���_��ݒ�*/
		void SetVertexes(const std::vector<Vector2Dd>& vs);
	private:
		std::vector<Vector2Dd> points_;
		std::vector<Vector2Dd> vertexes_;
		std::vector<std::array<int, 3>> indexes_;
		void UpdateVertex_();
		void UpdatePorygonIndex_();
		/*�`�揈��*/
		void DrawProc(ScreenDrawer2D& drawer)override;
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CDrawPolygon2D);
}