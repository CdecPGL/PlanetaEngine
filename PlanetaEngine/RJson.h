#pragma once

#include "JsonFile.h"
#include "ResourceBase.h"

namespace planeta {
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