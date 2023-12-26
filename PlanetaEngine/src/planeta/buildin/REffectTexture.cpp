#pragma once

#pragma warning(push)
#pragma warning(disable: 4100)
#include "Effekseer.h"
#pragma warning(pop)

#include "planeta/core/File.hpp"

#include "REffectTexture.hpp"

plnt::REffectTexture::REffectTexture() = default;
plnt::REffectTexture::~REffectTexture() = default;

bool plnt::REffectTexture::OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) {
	dx_base_image_ = std::unique_ptr<::DxLib::BASEIMAGE>(new ::DxLib::BASEIMAGE{});
	if (CreateBaseImageToMem(file.top_pointer(), file.size(), dx_base_image_.get(), false)) {
		PE_LOG_ERROR("テクスチャの作成に失敗しました。");
		dx_base_image_.release();
		return false;
	} else {
		// TODO: 最新化
		//effekseer_taxture_ = std::make_unique<::Effekseer::TextureData>();
		//effekseer_taxture_->Height = dx_base_image_->Height;
		//effekseer_taxture_->Width = dx_base_image_->Width;
		//effekseer_taxture_->UserPtr = dx_base_image_->GraphData;
		return true;
	}
	/*effekseer_taxture_ = std::move(planeta::private_::CreateEffekseerTextureDataFromFile(file));
	return effekseer_taxture_ != nullptr;*/
}

void plnt::REffectTexture::OnDisposed() {}


::Effekseer::TextureRef plnt::REffectTexture::effekseer_taxture() const { return effekseer_taxture_; }
