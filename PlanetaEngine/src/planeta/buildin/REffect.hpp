#pragma once
#include "planeta/core/ResourceBase.hpp"

namespace Effekseer {
	class Effect;
}
namespace plnt {
	class REffect : public ResourceBase {
	public:
		REffect() {};
		~REffect() {};
		Effekseer::Effect* effekseer_effect()const;
	private:
		virtual bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		virtual void OnDisposed() override;
		Effekseer::Effect* effekseer_effect_ = nullptr;
	};
}