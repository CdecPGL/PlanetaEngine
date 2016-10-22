#include "EffekseerUtil.h"
#include "CharacterCode.h"

namespace planeta {
	namespace private_ {
		bool EffectLoaderForEffekseer::Load(const EFK_CHAR*, void*& data, int32_t& size) {
			PE_LOG_MESSAGE("EffekseerEffectLoader���s");
			assert(file_ != nullptr);
			size = static_cast<int32_t>(file_->size());
			data = new uint8_t[size];
			if (memcpy_s(data, size, file_->top_pointer(), size)) {
				PE_LOG_ERROR("�f�[�^�̃R�s�[�Ɏ��s���܂����B�t�@�C���p�X��\"", file_->file_name(), "\"�B");
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

		void* TextureLoaderForEffekseer::Load(const EFK_CHAR* path) {
			PE_LOG_MESSAGE("EffekseerTextureLoader���s");
			auto tex = texture_getter_(util::ConvertUTF16ToSystemCode(reinterpret_cast<const wchar_t*>(path)));
			return tex;
		}

		void TextureLoaderForEffekseer::Unload(void* data) {

		}

		void TextureLoaderForEffekseer::SetTextureGetter(const std::function<void*(const std::string&)>& func) {
			texture_getter_ = func;
		}

	}
}