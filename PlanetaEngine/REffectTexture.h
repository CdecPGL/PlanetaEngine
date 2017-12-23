#pragma once

#include "ResourceBase.h"
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
namespace planeta {
	class REffectTexture final: public ResourceBase {
	public:
		REffectTexture();
		~REffectTexture();
		::Effekseer::TextureData* effekseer_taxture()const;
	private:
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
		
		std::unique_ptr<::Effekseer::TextureData> effekseer_taxture_;
		std::unique_ptr<::DxLib::BASEIMAGE> dx_base_image_;
	};
}