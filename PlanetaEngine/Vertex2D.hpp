#pragma once
#include "Color.h"
#include "Vector2D.h"


namespace planeta {
	namespace util {
		/*2D頂点構造体*/
		struct Vertex2D {
			Vector2Df position; //座標
			Color color; //ディヒューズカラー
			float u = 0.0f; //テクスチャ座用U
			float v = 0.0f; //テクスチャ座標V
		};
	}
}