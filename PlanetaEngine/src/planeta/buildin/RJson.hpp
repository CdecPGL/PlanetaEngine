#pragma once

#include "planeta/core/JsonFile.hpp"
#include "planeta/core/ResourceBase.hpp"

namespace plnt {
	/*! @brief JSONファイルを扱うリソースクラス
	*/
	class RJson final : public ResourceBase {
	public:
		const json_file &get_json_file() const;
		json_file &get_json_file();

	private:
		std::unique_ptr<json_file > json_file_;
		bool OnLoaded(const file &file, const json_file  &metadata, ResourceReferencer &referencer) override;
		void OnDisposed() override;
	};
}
