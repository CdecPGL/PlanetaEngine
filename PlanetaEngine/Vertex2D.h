#pragma once
#include "Color.h"
#include "Vector2D.h"


namespace planeta_engine {
	namespace utility {
		/*2D頂点構造体*/
		struct Vertex2D {
			Vector2D<float> position; //座標
			Color color; //ディヒューズカラー
			float u = 0.0f; //テクスチャ座用U
			float v = 0.0f; //テクスチャ座標V
		};
	}
}