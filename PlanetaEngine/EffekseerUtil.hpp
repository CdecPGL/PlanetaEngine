#pragma once

#include <functional>
#include "File.h"
#include "EffekseerForDXLib.h"

#define EFFEKSEER_FOR_DXLIB_VERSION 122

namespace planeta {
	namespace private_ {
		//Load関数の引数は使わずに、読み込み直前にFileを指定して、それを用いて読み込みを行う。
		class EffectLoaderForEffekseer final: public ::Effekseer::EffectLoader {
		public:
			void SetFile(const File* file);
			//事前に指定されたFileからデータのコピーを行う
			bool Load(const EFK_CHAR* path, void*& data, int32_t& size) override;
			void Unload(void* data, int32_t size) override;
		private:
			const File* file_ = nullptr;
		};
		//読み込み前にテクスチャ取得用のコールバック関数を設定し、それを用いてテクスチャを取得する。
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
