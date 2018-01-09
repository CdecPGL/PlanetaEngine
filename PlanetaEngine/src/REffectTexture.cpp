#include "REffectTexture.hpp"
#include "File.hpp"

#if EFFEKSEER_FOR_DXLIB_VERSION >= 130

#include "Effekseer.hpp"

planeta::REffectTexture::REffectTexture() = default;
planeta::REffectTexture::~REffectTexture() = default;

#else

#include "EffekseerForDXLib.hpp"
#include <d3dx9tex.h>

#endif

bool planeta::REffectTexture::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
#if EFFEKSEER_FOR_DXLIB_VERSION >= 130

	dx_base_image_ = std::unique_ptr<::DxLib::BASEIMAGE>(new ::DxLib::BASEIMAGE{});
	if (CreateBaseImageToMem(file.top_pointer(), file.size(), dx_base_image_.get(), false)) {
		PE_LOG_ERROR("�e�N�X�`���̍쐬�Ɏ��s���܂����B");
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

#else

	auto ret = D3DXCreateTextureFromFileInMemory(GetEffekseer3DRenderer()->GetDevice(), file.top_pointer(), file.size(), &texture_d9_);
	switch (ret) {
	case D3D_OK:
		return true;
	case D3DERR_NOTAVAILABLE:
		PE_LOG_ERROR("�e�N�X�`���̍쐬�Ɏ��s���܂����B�T�|�[�g����Ă��Ȃ��@�\�ł��B");
		return false;
	case D3DERR_OUTOFVIDEOMEMORY:
		PE_LOG_ERROR("�e�N�X�`���̍쐬�Ɏ��s���܂����B�f�B�X�v���C������������܂���B");
		return false;
	case D3DERR_INVALIDCALL:
		PE_LOG_ERROR("�e�N�X�`���̍쐬�Ɏ��s���܂����B�����������ł���\��������܂��B");
		return false;
	case D3DXERR_INVALIDDATA:
		PE_LOG_ERROR("�e�N�X�`���̍쐬�Ɏ��s���܂����B�f�[�^�������ł��B");
		return false;
	case E_OUTOFMEMORY:
		PE_LOG_ERROR("�e�N�X�`���̍쐬�Ɏ��s���܂����B������������܂���B");
		return false;
	default:
		PE_LOG_ERROR("�e�N�X�`���̍쐬�Ɏ��s���܂����B�����s���̃G���[�ł��B");
		return false;
	}

#endif
}

void planeta::REffectTexture::OnDisposed() {
#if not EFFEKSEER_FOR_DXLIB_VERSION >= 130
	if (texture_d9_) { texture_d9_->Release(); }
#endif
}

#if EFFEKSEER_FOR_DXLIB_VERSION >= 130

::Effekseer::TextureData* planeta::REffectTexture::effekseer_taxture()const {
	return effekseer_taxture_.get();
}

#else

IDirect3DTexture9* planeta::REffectTexture::texture_dx9()const {
	return texture_d9_;
}

#endif
