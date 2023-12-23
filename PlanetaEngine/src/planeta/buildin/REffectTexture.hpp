#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "planeta/effekseer/EffekseerUtil.hpp"

#include "DxLib.h"


namespace std {
	template<>
	struct default_delete<::DxLib::BASEIMAGE> {
		void operator()(::DxLib::BASEIMAGE* dx_base_image) const {
			ReleaseBaseImage(dx_base_image);
		}
	};
};


namespace plnt {
	class REffectTexture final: public ResourceBase {
	public:
		REffectTexture();
		~REffectTexture();
		::Effekseer::TextureRef effekseer_taxture()const;
	private:
		::Effekseer::TextureRef effekseer_taxture_;
		std::unique_ptr<::DxLib::BASEIMAGE> dx_base_image_;
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
	};
}