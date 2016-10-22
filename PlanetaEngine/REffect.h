#pragma once
#include "ResourceBase.h"

namespace Effekseer {
	class Effect;
}
namespace planeta {
	class REffect : public private_::ResourceBase {
	public:
		REffect() {};
		~REffect() {};
		Effekseer::Effect* effekseer_effect()const;
	private:
		virtual bool _Create(const File& file, private_::ResourceReferencer&) override;
		virtual void _Dispose() override;
		Effekseer::Effect* effekseer_effect_ = nullptr;
	};
}