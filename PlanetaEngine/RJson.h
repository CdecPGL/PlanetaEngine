#pragma once

#include "JsonFile.h"
#include "ResourceBase.h"

namespace planeta {
	/*! @brief JSONファイルを扱うリソースクラス
	*/
	class RJson final : public private_::ResourceBase {
	public:
		const JsonFile& json_file()const;
		JsonFile& json_file();
	private:
		std::unique_ptr<JsonFile> json_file_;
		bool _Create(const File& file, private_::ResourceReferencer&) override;
		void _Dispose() override;
	};
}