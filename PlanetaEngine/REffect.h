#pragma once
#include "ResourceBase.h"

namespace planeta {
	class REffect : public private_::ResourceBase {
	public:
		REffect() {};
		~REffect() {};
		int effect_handle()const;
	private:
		virtual bool _Create(const File& file, private_::ResourceReferencer&) override;
		virtual void _Dispose() override;
		int effect_handle_ = -1;
	};
}