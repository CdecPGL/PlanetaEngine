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
	class effect_loader_for_effekseer final : public ::Effekseer::EffectLoader {
	public:
		void set_file(const File *file);
		//事前に指定されたFileからデータのコピーを行う
		bool Load(const EFK_CHAR *path, void *&data, int32_t &size) override;
		void Unload(void *data, int32_t size) override;

	private:
		const File *file_ = nullptr;
	};

	//読み込み前にテクスチャ取得用のコールバック関数を設定し、それを用いてテクスチャを取得する。
	//REffectTextureで削除するのでUnloadでの削除は行わない
	class texture_loader_for_effekseer final : public ::Effekseer::TextureLoader {
	public:
		using texture_getter_type = std::function<::Effekseer::TextureRef
			(const std::string &, ::Effekseer::TextureType)>;
		::Effekseer::TextureRef Load(const EFK_CHAR *path, ::Effekseer::TextureType texture_type) override;
		texture_loader_for_effekseer() = default;
		void set_texture_getter(const texture_getter_type &func);

	private:
		texture_getter_type texture_getter_;
	};

	::Effekseer::TextureRef create_effekseer_texture_data_from_file(const File &file);
}
