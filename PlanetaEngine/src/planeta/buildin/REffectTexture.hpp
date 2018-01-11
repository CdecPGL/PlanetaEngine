#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "planeta/effekseer/EffekseerUtil.hpp"

#if EFFEKSEER_FOR_DXLIB_VERSION >= 130

#include "DxLib.h"

namespace Effekseer {
	struct TextureData;
}
namespace std {
	template<>
	struct default_delete<::DxLib::BASEIMAGE> {
		void operator()(::DxLib::BASEIMAGE* dx_base_image) const {
			ReleaseBaseImage(dx_base_image);
		}
	};
};

#else
struct IDirect3DTexture9;
#endif

namespace plnt {
	class REffectTexture final: public ResourceBase {
	public:
#if EFFEKSEER_FOR_DXLIB_VERSION >= 130
		REffectTexture();
		~REffectTexture();
		::Effekseer::TextureData* effekseer_taxture()const;
	private:
		std::unique_ptr<::Effekseer::TextureData> effekseer_taxture_;
		std::unique_ptr<::DxLib::BASEIMAGE> dx_base_image_;
#else
		IDirect3DTexture9* texture_dx9()const;
	private:
		IDirect3DTexture9 * texture_d9_ = nullptr;
#endif
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
	};
}