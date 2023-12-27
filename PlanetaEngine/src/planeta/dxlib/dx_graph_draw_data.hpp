#pragma once

#include <memory>

#include "DxLib.h"

namespace plnt::dxlib {
	struct dx_graph_draw_data {
		std::unique_ptr<VERTEX3D[]> vertexes; //頂点情報
		size_t vertex_count = 0; //頂点数
		std::unique_ptr<unsigned short[]> indexes; //インデックス情報
		size_t polygon_count = 0; //ポリゴン数
		int graph_handle = -1; //画像ハンドル
	};
}
