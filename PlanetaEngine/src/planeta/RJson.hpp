#pragma once

#include "planeta/JsonFile.hpp"
#include "ResourceBase.hpp"

namespace plnt {
	/*! @brief JSONファイルを扱うリソースクラス
	*/
	class RJson final : public ResourceBase {
	public:
		const JsonFile& json_file()const;
		JsonFile& json_file();
	private:
		std::unique_ptr<JsonFile> json_file_;
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
	};
}