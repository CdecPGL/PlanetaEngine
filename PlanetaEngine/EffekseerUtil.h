#pragma once

#include <functional>
#include "File.h"
#include "EffekseerForDXLib.h"

#define EFFEKSEER_FOR_DXLIB_VERSION 122

namespace planeta {
	namespace private_ {
		//Load�֐��̈����͎g�킸�ɁA�ǂݍ��ݒ��O��File���w�肵�āA�����p���ēǂݍ��݂��s���B
		class EffectLoaderForEffekseer final: public ::Effekseer::EffectLoader {
		public:
			void SetFile(const File* file);
			//���O�Ɏw�肳�ꂽFile����f�[�^�̃R�s�[���s��
			bool Load(const EFK_CHAR* path, void*& data, int32_t& size) override;
			void Unload(void* data, int32_t size) override;
		private:
			const File* file_ = nullptr;
		};
		//�ǂݍ��ݑO�Ƀe�N�X�`���擾�p�̃R�[���o�b�N�֐���ݒ肵�A�����p���ăe�N�X�`�����擾����B
		class TextureLoaderForEffekseer final : public ::Effekseer::TextureLoader {
		public:
#if EFFEKSEER_FOR_DXLIB_VERSION >= 130
			using TextureGetterType = std::function<::Effekseer::TextureData*(const std::string&, ::Effekseer::TextureType)>;
			::Effekseer::TextureData* Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType)override;
			void Unload(::Effekseer::TextureData* data);
#else
			using TextureGetterType = std::function<void*(const std::string&)>;
			void* Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType)override;
			void Unload(void* data);
#endif
			TextureLoaderForEffekseer();
			void SetTextureGetter(const TextureGetterType& func);
		private:
			TextureGetterType texture_getter_;
		};

#if EFFEKSEER_FOR_DXLIB_VERSION >= 130
		std::unique_ptr<::Effekseer::TextureData> CreateEffekseerTextureDataFromFile(const File& file);
#endif
	}
}
