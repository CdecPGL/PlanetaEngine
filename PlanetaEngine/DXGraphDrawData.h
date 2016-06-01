#pragma once

#include <memory>
#include "DxLib.h"

namespace planeta {
	namespace core {
		struct DXGraphDrawData{
			std::unique_ptr<VERTEX3D[]> vertexes; //���_���
			size_t vertex_count = 0; //���_��
			std::unique_ptr<unsigned short[]> indexes; //�C���f�b�N�X���
			size_t polygon_count = 0; //�|���S����
			int graph_handle = -1; //�摜�n���h��
		};
	}
}