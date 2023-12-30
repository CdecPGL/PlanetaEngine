#pragma once

#include "color.hpp"
#include "Vector2D.hpp"

namespace plnt::util {
	/*2D頂点構造体*/
	struct vertex_2d {
		vector_2df position; //座標
		color color; //ディヒューズカラー
		float u = 0.0f; //テクスチャ座用U
		float v = 0.0f; //テクスチャ座標V
	};
}
