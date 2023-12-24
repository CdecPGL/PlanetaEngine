#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "planeta/core/File.hpp"
#include "planeta/effekseer/EffekseerUtil.hpp"

#include "REffect.hpp"
#include "REffectTexture.hpp"

using namespace ::plnt::effekseer;

Effekseer::EffectRef plnt::REffect::effekseer_effect() const {
	return effekseer_effect_;
}

bool plnt::REffect::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
	//�ǂݍ��ݗp�ݒ�
	decltype(auto) eff_mgr = GetEffekseer3DManager();
	//�G�t�F�N�g���[�_�[�̐ݒ�
	/*decltype(auto) eff_ldr = eff_mgr->GetEffectLoader();
	assert(dynamic_cast<private_::EffectLoaderForEffekseer*>(eff_ldr) != nullptr);
	static_cast<private_::EffectLoaderForEffekseer*>(eff_ldr)->SetFile(&file);*/
	//�e�N�X�`�����[�_�[�̐ݒ�
	eff_mgr->SetTextureLoader(::Effekseer::MakeRefPtr<TextureLoaderForEffekseer>());
	decltype(auto) txr_ldr = eff_mgr->GetTextureLoader();
	assert(txr_ldr != nullptr);

	txr_ldr.DownCast<TextureLoaderForEffekseer>()->SetTextureGetter([&referencer] (const std::string& path, ::Effekseer::TextureType texture_type)->::Effekseer::TextureRef{
		auto tex_res = referencer.ReferenceResourceByPath<REffectTexture>(path);
		if (tex_res) {
			return tex_res->effekseer_taxture();
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

void plnt::REffect::OnDisposed() {
	if (effekseer_effect_ != nullptr) {
		effekseer_effect_->Release();
	}
}
