#include "REffectTexture.h"
#include "Effekseer.h"
#include "File.h"

planeta::REffectTexture::REffectTexture() = default;
planeta::REffectTexture::~REffectTexture() = default;

bool planeta::REffectTexture::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
	dx_base_image_ = std::unique_ptr<::DxLib::BASEIMAGE>(new ::DxLib::BASEIMAGE{});
	if (CreateBaseImageToMem(file.top_pointer(), file.size(), dx_base_image_.get(), false)) {
		PE_LOG_ERROR("テクスチャの作成に失敗しました。");
		dx_base_image_.release();
		return false;
	}
	else {
		effekseer_taxture_ = std::make_unique<::Effekseer::TextureData>();
		effekseer_taxture_->Height = dx_base_image_->Height;
		effekseer_taxture_->Width = dx_base_image_->Width;
		effekseer_taxture_->UserPtr = dx_base_image_->GraphData;
		return true;
	}

	/*effekseer_taxture_ = std::move(planeta::private_::CreateEffekseerTextureDataFromFile(file));
	return effekseer_taxture_ != nullptr;*/
	/*switch (ret) {
	case D3D_OK:
		return true;
	case D3DERR_NOTAVAILABLE:
		PE_LOG_ERROR("テクスチャの作成に失敗しました。サポートされていない機能です。");
		return false;
	case D3DERR_OUTOFVIDEOMEMORY:
		PE_LOG_ERROR("テクスチャの作成に失敗しました。ディスプレイメモリが足りません。");
		return false;
	case D3DERR_INVALIDCALL:
		PE_LOG_ERROR("テクスチャの作成に失敗しました。引数が無効である可能性があります。");
		return false;
	case D3DXERR_INVALIDDATA:
		PE_LOG_ERROR("テクスチャの作成に失敗しました。データが無効です。");
		return false;
	case E_OUTOFMEMORY:
		PE_LOG_ERROR("テクスチャの作成に失敗しました。メモリが足りません。");
		return false;
	default:
		PE_LOG_ERROR("テクスチャの作成に失敗しました。原因不明のエラーです。");
		return false;
	}*/
}

void planeta::REffectTexture::OnDisposed() {}

::Effekseer::TextureData* planeta::REffectTexture::effekseer_taxture()const {
	return effekseer_taxture_.get();
}
