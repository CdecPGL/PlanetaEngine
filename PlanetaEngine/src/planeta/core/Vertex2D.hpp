#pragma once
#include "color.hpp"
#include "Vector2D.hpp"


namespace plnt {
	namespace util {
		/*2D頂点構造体*/
		struct Vertex2D {
			Vector2Df position; //座標
			color color; //ディヒューズカラー
			float u = 0.0f; //テクスチャ座用U
			float v = 0.0f; //テクスチャ座標V
		};
	}
}
