#pragma once

#include <functional>
#include "File.h"
#include "EffekseerForDXLib.h"

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
		//�ǂݍ��ݑO�Ƀt�@�C���擾�p�̃R�[���o�b�N�֐���ݒ肵�A�����p���ăt�@�C�����擾����B
		class TextureLoaderForEffekseer final : public ::Effekseer::TextureLoader {
		public:
			TextureLoaderForEffekseer(::EffekseerRenderer::Renderer* renderer);
			void SetCaIllBackFunctionOnLoad(const std::function<IDirect3DTexture9*(const std::string&)>& func);
			void* Load(const EFK_CHAR* path);
			void Unload(void* data);
		private:
			::EffekseerRenderer::Renderer*	m_renderer;
			std::function<IDirect3DTexture9*(const std::string&)> call_back_func_on_load_;
		};
	}
}