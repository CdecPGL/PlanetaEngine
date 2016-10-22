#pragma once

#include "ResourceBase.h"

struct IDirect3DTexture9;
namespace planeta {
	class REffectTexture final: public private_::ResourceBase {
	public:
		IDirect3DTexture9* texture_dx9()const;
	private:
		bool _Create(const File& file, private_::ResourceReferencer&) override;
		void _Dispose() override;
		IDirect3DTexture9* texture_d9_ = nullptr;
	};
}