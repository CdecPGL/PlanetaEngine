#pragma once

#include "GGameObject2DTemplate.h"

namespace planeta {
	class GParticleEmitter : public GGameObject2DTemplate<> {
	public:
		using Super = GGameObject2DTemplate;
	};
	PE_GAMEOBJECT_CLASS(GParticleEmitter);
}