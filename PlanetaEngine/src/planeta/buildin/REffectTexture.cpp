#pragma warning(push)
#pragma warning(disable: 4100)
#include "Effekseer.h"
#pragma warning(pop)

#include "planeta/core/file.hpp"

#include "REffectTexture.hpp"

bool plnt::r_effect_texture::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
	dx_base_image_ = std::make_unique<DxLib::BASEIMAGE>(::DxLib::BASEIMAGE{});
	if (CreateBaseImageToMem(file.top_pointer(), file.size(), dx_base_image_.get(), false)) {
		PE_LOG_ERROR("テクスチャの作成に失敗しました。");
		dx_base_image_.release();
		return false;
	}
	// TODO: 最新化
	//effekseer_texture_ = std::make_unique<::Effekseer::TextureData>();
	//effekseer_texture_->Height = dx_base_image_->Height;
	//effekseer_texture_->Width = dx_base_image_->Width;
	//effekseer_texture_->UserPtr = dx_base_image_->GraphData;
	return true;
	/*effekseer_texture_ = std::move(planeta::private_::CreateEffekseerTextureDataFromFile(file));
	return effekseer_texture_ != nullptr;*/
}

void plnt::r_effect_texture::on_disposed() {}


::Effekseer::TextureRef plnt::r_effect_texture::effekseer_texture() const { return effekseer_texture_; }
