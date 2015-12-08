#pragma once
#include "Color.h"
#include "Vector2D.h"


namespace planeta_engine {
	namespace utility {
		/*2D���_�\����*/
		struct Vertex2D {
			math::Vector2Df position; //���W
			core::Color color; //�f�B�q���[�Y�J���[
			float u = 0.0f; //�e�N�X�`�����pU
			float v = 0.0f; //�e�N�X�`�����WV
		};
	}
}