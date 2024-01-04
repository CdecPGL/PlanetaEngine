#pragma once

#include "DxLib.h"

#include "../core/resource_base.hpp"
#include "../effekseer/effekseer_util.hpp"

template <>
struct std::default_delete<::DxLib::BASEIMAGE> {
	void operator()(::DxLib::BASEIMAGE *dx_base_image) const { ReleaseBaseImage(dx_base_image); }
};


namespace plnt {
	class r_effect_texture final : public resource_base {
	public:
		::Effekseer::TextureRef effekseer_texture() const;

	private:
		::Effekseer::TextureRef effekseer_texture_;
		std::unique_ptr<::DxLib::BASEIMAGE> dx_base_image_;
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
	};
}
