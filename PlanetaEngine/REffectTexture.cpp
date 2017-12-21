#include "REffectTexture.h"
#include "File.h"
#include "EffekseerForDXLib.h"
#include <d3dx9tex.h>

//#pragma comment(lib,"d3dx9.lib")

bool planeta::REffectTexture::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
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
}

void planeta::REffectTexture::OnDisposed() {
	if (texture_d9_) { texture_d9_->Release(); }
}

IDirect3DTexture9* planeta::REffectTexture::texture_dx9()const {
	return texture_d9_;
}
