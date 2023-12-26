#pragma once

#include <functional>

#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "planeta/core/File.hpp"

// TODO: �ŐV��EffekseerForDXLib�Ή�

namespace plnt::effekseer {
	//Load�֐��̈����͎g�킸�ɁA�ǂݍ��ݒ��O��File���w�肵�āA�����p���ēǂݍ��݂��s���B
	class EffectLoaderForEffekseer final : public ::Effekseer::EffectLoader {
	public:
		void SetFile(const File *file);
		//���O�Ɏw�肳�ꂽFile����f�[�^�̃R�s�[���s��
		bool Load(const EFK_CHAR *path, void *&data, int32_t &size) override;
		void Unload(void *data, int32_t size) override;

	private:
		const File *file_ = nullptr;
	};

	//�ǂݍ��ݑO�Ƀe�N�X�`���擾�p�̃R�[���o�b�N�֐���ݒ肵�A�����p���ăe�N�X�`�����擾����B
	//REffectTexture�ō폜����̂�Unload�ł̍폜�͍s��Ȃ�
	class TextureLoaderForEffekseer final : public ::Effekseer::TextureLoader {
	public:
		using TextureGetterType = std::function<::Effekseer::TextureRef(const std::string &, ::Effekseer::TextureType)>;
		::Effekseer::TextureRef Load(const EFK_CHAR *path, ::Effekseer::TextureType textureType) override;
		TextureLoaderForEffekseer();
		void SetTextureGetter(const TextureGetterType &func);

	private:
		TextureGetterType texture_getter_;
	};

	::Effekseer::TextureRef CreateEffekseerTextureDataFromFile(const File &file);
}
