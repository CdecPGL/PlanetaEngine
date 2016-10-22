#include "REffect.h"
#include "File.h"
#include "ResourceManager.h"

#include "EffekseerForDXLib.h"
#include "EffekseerUtil.h"
#include "REffectTexture.h"

int planeta::REffect::effect_handle() const {
	return effect_handle_;
}

bool planeta::REffect::_Create(const File& file, private_::ResourceReferencer& referencer) {
	//読み込み用設定
	decltype(auto) eff_mgr = GetEffekseer3DManager();
	//エフェクトローダーの設定
	decltype(auto) eff_ldr = eff_mgr->GetEffectLoader();
	assert(dynamic_cast<private_::EffectLoaderForEffekseer*>(eff_ldr) != nullptr);
	static_cast<private_::EffectLoaderForEffekseer*>(eff_ldr)->SetFile(&file);
	//テクスチャローダーの設定
	decltype(auto) txr_ldr = eff_mgr->GetTextureLoader();
	assert(dynamic_cast<private_::TextureLoaderForEffekseer*>(txr_ldr) != nullptr);
	static_cast<private_::TextureLoaderForEffekseer*>(txr_ldr)->SetTextureGetter([&referencer] (const std::string& path)->void*{
		auto tex_res = referencer.ReferenceResourceByPath<REffectTexture>(path);
		if (tex_res) {
			return tex_res->texture_dx9();
		} else {
			PE_LOG_ERROR("テクスチャリソースの読み込みに失敗しました。(指定パス:", path, ")");
			return nullptr;
		}
	});
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
