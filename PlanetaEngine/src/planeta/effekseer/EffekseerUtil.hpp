#pragma once

#include <functional>

#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "planeta/core/File.hpp"

// TODO: 最新のEffekseerForDXLib対応

namespace plnt::effekseer {
	//Load関数の引数は使わずに、読み込み直前にFileを指定して、それを用いて読み込みを行う。
	class EffectLoaderForEffekseer final : public ::Effekseer::EffectLoader {
	public:
		void SetFile(const File *file);
		//事前に指定されたFileからデータのコピーを行う
		bool Load(const EFK_CHAR *path, void *&data, int32_t &size) override;
		void Unload(void *data, int32_t size) override;

	private:
		const File *file_ = nullptr;
	};

	//読み込み前にテクスチャ取得用のコールバック関数を設定し、それを用いてテクスチャを取得する。
	//REffectTextureで削除するのでUnloadでの削除は行わない
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
