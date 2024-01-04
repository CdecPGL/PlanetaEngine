#pragma once

#pragma warning(push)
#pragma warning(disable : 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "../core/resource_base.hpp"

// ReSharper disable once CppInconsistentNaming
namespace Effekseer {
	// ReSharper disable once CppInconsistentNaming
	class Effect;
}

namespace plnt {
	class r_effect final : public resource_base {
	public:
		[[nodiscard]] Effekseer::EffectRef effekseer_effect() const;

	private:
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		Effekseer::EffectRef effekseer_effect_ = nullptr;
	};
}
