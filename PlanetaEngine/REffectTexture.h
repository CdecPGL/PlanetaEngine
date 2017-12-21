#pragma once

#include "ResourceBase.h"

struct IDirect3DTexture9;
namespace planeta {
	class REffectTexture final: public ResourceBase {
	public:
		IDirect3DTexture9* texture_dx9()const;
	private:
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
		IDirect3DTexture9* texture_d9_ = nullptr;
	};
}