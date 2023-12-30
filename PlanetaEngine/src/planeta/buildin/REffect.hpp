#pragma once

#pragma warning(push)
#pragma warning(disable: 4100)
#include "Effekseer.h"
#pragma warning(pop)

#include "planeta/core/ResourceBase.hpp"

namespace Effekseer {
	class Effect;
}

namespace plnt {
	class REffect : public resource_base {
	public:
		REffect() { };

		~REffect() { };
		Effekseer::EffectRef effekseer_effect() const;

	private:
		virtual bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		virtual void on_disposed() override;
		Effekseer::EffectRef effekseer_effect_ = nullptr;
	};
}
