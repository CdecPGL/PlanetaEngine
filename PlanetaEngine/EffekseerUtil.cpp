#include "EffekseerUtil.h"

namespace planeta {
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

		TextureLoaderForEffekseer::TextureLoaderForEffekseer(::EffekseerRenderer::Renderer* renderer):m_renderer(renderer) {}

		void* TextureLoaderForEffekseer::Load(const EFK_CHAR* path) {
			/*IDirect3DTexture9* ret = call_back_func_on_load_(path);
			if (ret) {
				return ret;
			} else {
				PE_LOG_ERROR("テクスチャの読み込みに失敗しました。テクスチャパスは\"", path, "\"。");
				return nullptr;
			}*/
			return nullptr;
		}

		void TextureLoaderForEffekseer::Unload(void* data) {

		}

		void TextureLoaderForEffekseer::SetCaIllBackFunctionOnLoad(const std::function<IDirect3DTexture9*(const std::string&)>& func) {
			call_back_func_on_load_ = func;
		}

	}
}