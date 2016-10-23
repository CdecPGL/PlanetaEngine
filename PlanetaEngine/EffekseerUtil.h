#pragma once

#include <functional>
#include "File.h"
#include "EffekseerForDXLib.h"

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
			TextureLoaderForEffekseer();
			void SetTextureGetter(const std::function<void*(const std::string&)>& func);
			void* Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType)override;
			void Unload(void* data);
		private:
			std::function<void*(const std::string&)> texture_getter_;
		};
	}
}