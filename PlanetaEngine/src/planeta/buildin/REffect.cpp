#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "planeta/core/File.hpp"
#include "..\effekseer\effekseer_util.hpp"

#include "REffect.hpp"
#include "REffectTexture.hpp"

using namespace ::plnt::effekseer;

Effekseer::EffectRef plnt::REffect::effekseer_effect() const { return effekseer_effect_; }

bool plnt::REffect::OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) {
	//読み込み用設定
	decltype(auto) eff_mgr = GetEffekseer3DManager();
	//エフェクトローダーの設定
	/*decltype(auto) eff_ldr = eff_mgr->GetEffectLoader();
	assert(dynamic_cast<private_::EffectLoaderForEffekseer*>(eff_ldr) != nullptr);
	static_cast<private_::EffectLoaderForEffekseer*>(eff_ldr)->SetFile(&file);*/
	//テクスチャローダーの設定
	eff_mgr->SetTextureLoader(::Effekseer::MakeRefPtr<texture_loader_for_effekseer>());
	decltype(auto) txr_ldr = eff_mgr->GetTextureLoader();
	assert(txr_ldr != nullptr);

	txr_ldr.DownCast<texture_loader_for_effekseer>()->set_texture_getter(
		[&referencer](const std::string &path, ::Effekseer::TextureType texture_type)-> ::Effekseer::TextureRef {
			auto tex_res = referencer.ReferenceResourceByPath<REffectTexture>(path);
			if (tex_res) { return tex_res->effekseer_taxture(); } else {
				PE_LOG_ERROR("エフェクトファイル内で参照されているテクスチャリソースの読み込みに失敗しました。(指定パス:", path, ")");
				return nullptr;
			}
		});
	//読み込み
	auto effect = Effekseer::Effect::Create(GetEffekseer3DManager(), const_cast<unsigned char *>(file.top_pointer()),
	                                        file.size());
	if (effect == nullptr) {
		PE_LOG_ERROR("Effekseerファイルの読み込みに失敗しました。");
		return false;
	}
	effekseer_effect_ = effect;
	return true;
}

void plnt::REffect::OnDisposed() { if (effekseer_effect_ != nullptr) { effekseer_effect_->Release(); } }
