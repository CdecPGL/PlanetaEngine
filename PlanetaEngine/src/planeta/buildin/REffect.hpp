#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "Effekseer.h"

namespace Effekseer {
	class Effect;
}
namespace plnt {
	class REffect : public ResourceBase {
	public:
		REffect() {};
		~REffect() {};
		Effekseer::EffectRef effekseer_effect()const;
	private:
		virtual bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		virtual void OnDisposed() override;
		Effekseer::EffectRef effekseer_effect_ = nullptr;
	};
}