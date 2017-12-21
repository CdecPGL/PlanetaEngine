#include "REffect.h"
#include "File.h"

#include "EffekseerForDXLib.h"
#include "EffekseerUtil.h"
#include "REffectTexture.h"

#include "EffekseerUtil.h"

Effekseer::Effect* planeta::REffect::effekseer_effect() const {
	return effekseer_effect_;
}

bool planeta::REffect::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
	//読み込み用設定
	decltype(auto) eff_mgr = GetEffekseer3DManager();
	//エフェクトローダーの設定
	/*decltype(auto) eff_ldr = eff_mgr->GetEffectLoader();
	assert(dynamic_cast<private_::EffectLoaderForEffekseer*>(eff_ldr) != nullptr);
	static_cast<private_::EffectLoaderForEffekseer*>(eff_ldr)->SetFile(&file);*/
	//テクスチャローダーの設定
	eff_mgr->SetTextureLoader(new private_::TextureLoaderForEffekseer);
	decltype(auto) txr_ldr = eff_mgr->GetTextureLoader();
	assert(dynamic_cast<private_::TextureLoaderForEffekseer*>(txr_ldr) != nullptr);
	static_cast<private_::TextureLoaderForEffekseer*>(txr_ldr)->SetTextureGetter([&referencer] (const std::string& path)->void*{
		auto tex_res = referencer.ReferenceResourceByPath<REffectTexture>(path);
		if (tex_res) {
			return tex_res->texture_dx9();
		} else {
			PE_LOG_ERROR("エフェクトファイル内で参照されているテクスチャリソースの読み込みに失敗しました。(指定パス:", path, ")");
			return nullptr;
		}
	});
	//読み込み
	auto effect = Effekseer::Effect::Create(GetEffekseer3DManager(), const_cast<unsigned char*>(file.top_pointer()), file.size());
	if (effect == nullptr) {
		PE_LOG_ERROR("Effekseerファイルの読み込みに失敗しました。");
		return false;
	}
	effekseer_effect_ = effect;
	return true;
}

void planeta::REffect::OnDisposed() {
	if (effekseer_effect_) {
		ES_SAFE_RELEASE(effekseer_effect_);
	}
}
