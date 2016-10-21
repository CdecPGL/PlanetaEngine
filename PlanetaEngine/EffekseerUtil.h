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
		//読み込み前にファイル取得用のコールバック関数を設定し、それを用いてファイルを取得する。
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