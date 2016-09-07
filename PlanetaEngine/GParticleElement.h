#pragma once

#include "GGameObject2DTemplate.h"

namespace planeta {
	class GParticleElement : public GGameObject2DTemplate<>{
	public:
		using Super = GGameObject2DTemplate;
	};
	PE_GAMEOBJECT_CLASS(GParticleElement);
}