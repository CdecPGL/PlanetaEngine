#include "REffect.h"
#include "File.h"
#include "ResourceManager.h"

#include "EffekseerForDXLib.h"
#include "EffekseerUtil.h"

int planeta::REffect::effect_handle() const {
	return effect_handle_;
}

bool planeta::REffect::_Create(const File& file) {
	//読み込み用設定
	decltype(auto) eff_mgr = GetEffekseer3DManager();
	//エフェクトローダーの設定
	decltype(auto) eff_ldr = eff_mgr->GetEffectLoader();
	assert(dynamic_cast<private_::EffectLoaderForEffekseer*>(eff_ldr) != nullptr);
	static_cast<private_::EffectLoaderForEffekseer*>(eff_ldr)->SetFile(&file);
	//テクスチャローダーの設定
	decltype(auto) txr_ldr = eff_mgr->GetTextureLoader();
	assert(dynamic_cast<private_::TextureLoaderForEffekseer*>(txr_ldr) != nullptr);
	static_cast<private_::TextureLoaderForEffekseer*>(txr_ldr);
	//読み込み
	int hdr = LoadEffekseerEffect(file.file_name().c_str());
	if (hdr < 0) {
		return false;
	}
	effect_handle_ = hdr;
	return true;
}

void planeta::REffect::_Dispose() {
	if (effect_handle_ >= 0) {
		DeleteEffekseerEffect(effect_handle_);
	}
}
