#pragma once
#include "Color.h"
#include "Vector2D.h"


namespace planeta {
	namespace util {
		/*2D���_�\����*/
		struct Vertex2D {
			Vector2Df position; //���W
			Color color; //�f�B�q���[�Y�J���[
			float u = 0.0f; //�e�N�X�`�����pU
			float v = 0.0f; //�e�N�X�`�����WV
		};
	}
}