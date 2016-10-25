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
		bool _Create(const File& file, ResourceReferencer&) override;
		void _Dispose() override;
	};
}