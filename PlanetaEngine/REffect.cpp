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
	//�ǂݍ��ݗp�ݒ�
	decltype(auto) eff_mgr = GetEffekseer3DManager();
	//�G�t�F�N�g���[�_�[�̐ݒ�
	/*decltype(auto) eff_ldr = eff_mgr->GetEffectLoader();
	assert(dynamic_cast<private_::EffectLoaderForEffekseer*>(eff_ldr) != nullptr);
	static_cast<private_::EffectLoaderForEffekseer*>(eff_ldr)->SetFile(&file);*/
	//�e�N�X�`�����[�_�[�̐ݒ�
	eff_mgr->SetTextureLoader(new private_::TextureLoaderForEffekseer);
	decltype(auto) txr_ldr = eff_mgr->GetTextureLoader();
	assert(dynamic_cast<private_::TextureLoaderForEffekseer*>(txr_ldr) != nullptr);
	static_cast<private_::TextureLoaderForEffekseer*>(txr_ldr)->SetTextureGetter([&referencer] (const std::string& path)->void*{
		auto tex_res = referencer.ReferenceResourceByPath<REffectTexture>(path);
		if (tex_res) {
			return tex_res->texture_dx9();
		} else {
			PE_LOG_ERROR("�G�t�F�N�g�t�@�C�����ŎQ�Ƃ���Ă���e�N�X�`�����\�[�X�̓ǂݍ��݂Ɏ��s���܂����B(�w��p�X:", path, ")");
			return nullptr;
		}
	});
	//�ǂݍ���
	auto effect = Effekseer::Effect::Create(GetEffekseer3DManager(), const_cast<unsigned char*>(file.top_pointer()), file.size());
	if (effect == nullptr) {
		PE_LOG_ERROR("Effekseer�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B");
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
