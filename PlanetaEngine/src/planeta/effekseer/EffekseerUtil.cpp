#include "planeta/effekseer/EffekseerUtil.hpp"
#include "planeta/core/CharacterCode.hpp"
#include "planeta/core/LogUtility.hpp"

#if EFFEKSEER_FOR_DXLIB_VERSION >= 130

#include "planeta/core/File.hpp"
#include "EffekseerForDXLib.h"
#include <d3dx9tex.h>

#endif

namespace plnt {
	namespace private_ {
		bool EffectLoaderForEffekseer::Load(const EFK_CHAR*, void*& data, int32_t& size) {
			assert(file_ != nullptr);
			size = static_cast<int32_t>(file_->size());
			data = new uint8_t[size];
			if (memcpy_s(data, size, file_->top_pointer(), size)) {
				PE_LOG_ERROR("データのコピーに失敗しました。ファイルパスは\"", file_->file_name(), "\"。");
				return false;
			}
			return true;
		}

		void EffectLoaderForEffekseer::Unload(void* data, int32_t size) {
			if (data != nullptr) { delete[] data; }
		}

		void EffectLoaderForEffekseer::SetFile(const File* file) {
			file_ = file;
		}

		TextureLoaderForEffekseer::TextureLoaderForEffekseer() {}

#if EFFEKSEER_FOR_DXLIB_VERSION >= 130
		::Effekseer::TextureData* TextureLoaderForEffekseer::Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType) {
			return texture_getter_(util::ConvertUTF16ToSystemCode(reinterpret_cast<const wchar_t*>(path)), textureType);
		}

		void TextureLoaderForEffekseer::Unload(::Effekseer::TextureData* data) {
			//REffectTextureで削除するのでここでの削除は行わない
		}
#else
		void* TextureLoaderForEffekseer::Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType) {
			return texture_getter_(util::ConvertUTF16ToSystemCode(reinterpret_cast<const wchar_t*>(path)));
		}

		void TextureLoaderForEffekseer::Unload(void* data) {
			//REffectTextureで削除するのでここでの削除は行わない
		}
#endif

		void TextureLoaderForEffekseer::SetTextureGetter(const TextureGetterType& func) {
			texture_getter_ = func;
		}

#if EFFEKSEER_FOR_DXLIB_VERSION >= 130
		std::unique_ptr<::Effekseer::TextureData> CreateEffekseerTextureDataFromFile(const File& file) {
			::Effekseer::TextureData* textureData = nullptr;
			DxLib::BASEIMAGE dx_base_image;
			CreateBaseImageToMem(file.top_pointer(), file.size(), &dx_base_image, false);
			textureData->Height = dx_base_image.Height;
			textureData->Width = dx_base_image.Width;
			textureData->UserPtr = dx_base_image.GraphData;
			//textureData->UserPtr = dx_base_image.ColorData;
			return std::unique_ptr<::Effekseer::TextureData>{ textureData };

		//	char* data_top = reinterpret_cast<char*>(const_cast<unsigned char*>(file.top_pointer()));
		//	//DirectX9デバイスの取得
		//	auto* effekseer_renderer = GetEffekseer3DRenderer();
		//	if (effekseer_renderer == nullptr) {
		//		PE_LOG_ERROR("描画デバイスの取得に失敗しました。Effekseerが初期化されていない可能性があります。");
		//		return false;
		//	}
		//	auto* effekseer_dx9_renderer = dynamic_cast<EffekseerRendererDX9::Renderer*>(effekseer_renderer);
		//	if (effekseer_dx9_renderer == nullptr) {
		//		PE_LOG_ERROR("DirectX9描画デバイスの取得に失敗しました。DirectX9以外のバージョンで動作している可能性があります。");
		//		return false;
		//	}
		//	auto* device = effekseer_dx9_renderer->GetDevice();
		//	//テクスチャの生成
		//	if (file.size() < 4) {
		//	}
		//	else if (
		//		data_top[1] == 'P' &&
		//		data_top[2] == 'N' &&
		//		data_top[3] == 'G') {
		//		if (::EffekseerRenderer::PngTextureLoader::Load(data_top, file.size(), false)) {
		//			HRESULT hr;
		//			int32_t width = ::EffekseerRenderer::PngTextureLoader::GetWidth();
		//			int32_t height = ::EffekseerRenderer::PngTextureLoader::GetHeight();
		//			int32_t mipMapCount = 1;
		//			hr = device->CreateTexture(
		//				width,
		//				height,
		//				mipMapCount,
		//				D3DUSAGE_AUTOGENMIPMAP,
		//				D3DFMT_A8R8G8B8,
		//				D3DPOOL_DEFAULT,
		//				&texture,
		//				NULL);

		//			if (FAILED(hr)) {
		//				::EffekseerRenderer::PngTextureLoader::Unload();
		//				goto Exit;
		//			}

		//			LPDIRECT3DTEXTURE9 tempTexture = NULL;
		//			hr = device->CreateTexture(
		//				width,
		//				height,
		//				mipMapCount,
		//				0,
		//				D3DFMT_A8R8G8B8,
		//				D3DPOOL_SYSTEMMEM,
		//				&tempTexture,
		//				NULL);

		//			if (FAILED(hr)) {
		//				::EffekseerRenderer::PngTextureLoader::Unload();
		//				goto Exit;
		//			}

		//			uint8_t* srcBits = (uint8_t*)::EffekseerRenderer::PngTextureLoader::GetData().data();
		//			D3DLOCKED_RECT locked;
		//			if (SUCCEEDED(tempTexture->LockRect(0, &locked, NULL, 0))) {
		//				uint8_t* destBits = (uint8_t*)locked.pBits;

		//				for (int32_t h = 0; h < height; h++) {
		//					memcpy(destBits, srcBits, width * 4);

		//					// RGB入れ替え
		//					for (int32_t w = 0; w < width; w++) {
		//						std::swap(destBits[w * 4 + 0], destBits[w * 4 + 2]);
		//					}

		//					destBits += locked.Pitch;
		//					srcBits += (width * 4);
		//				}

		//				tempTexture->UnlockRect(0);
		//			}

		//			hr = device->UpdateTexture(tempTexture, texture);
		//			ES_SAFE_RELEASE(tempTexture);

		//			::EffekseerRenderer::PngTextureLoader::Unload();

		//			textureData = new Effekseer::TextureData();
		//			textureData->UserPtr = texture;
		//			textureData->UserID = 0;
		//			textureData->TextureFormat = Effekseer::TextureFormatType::ABGR8;
		//			textureData->Width = width;
		//			textureData->Height = height;
		//		}
		//	}
		//	else if (data_top[0] == 'D' &&
		//		data_top[1] == 'D' &&
		//		data_top[2] == 'S' &&
		//		data_top[3] == ' ') {
		//		EffekseerDirectX::CreateDDSTextureFromMemory(
		//			device,
		//			(uint8_t*)data_top,
		//			file.size(),
		//			texture);

		//		// To get texture size, use loader
		//		EffekseerRenderer::DDSTextureLoader::Load(data_top, file.size());

		//		textureData = new Effekseer::TextureData();
		//		textureData->UserPtr = texture;
		//		textureData->UserID = 0;
		//		textureData->TextureFormat = EffekseerRenderer::DDSTextureLoader::GetTextureFormat();
		//		textureData->Width = EffekseerRenderer::DDSTextureLoader::GetWidth();
		//		textureData->Height = EffekseerRenderer::DDSTextureLoader::GetHeight();
		//	}

		//Exit:;
		//	return textureData;
		}
#endif
	}
}
