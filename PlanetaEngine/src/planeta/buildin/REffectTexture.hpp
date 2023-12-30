#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "..\effekseer\effekseer_util.hpp"

#include "DxLib.h"


namespace std {
	template <>
	struct default_delete<::DxLib::BASEIMAGE> {
		void operator()(::DxLib::BASEIMAGE *dx_base_image) const { ReleaseBaseImage(dx_base_image); }
	};
};


namespace plnt {
	class REffectTexture final : public resource_base {
	public:
		REffectTexture();
		~REffectTexture();
		::Effekseer::TextureRef effekseer_taxture() const;

	private:
		::Effekseer::TextureRef effekseer_taxture_;
		std::unique_ptr<::DxLib::BASEIMAGE> dx_base_image_;
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
	};
}
