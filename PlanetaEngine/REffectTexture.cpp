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
	}
}

void planeta::REffectTexture::OnDisposed() {
	if (texture_d9_) { texture_d9_->Release(); }
}

IDirect3DTexture9* planeta::REffectTexture::texture_dx9()const {
	return texture_d9_;
}
