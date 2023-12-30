#pragma once

#include "..\core\json_file.hpp"
#include "..\core\resource_base.hpp"

namespace plnt {
	/*! @brief JSONファイルを扱うリソースクラス
	*/
	class RJson final : public resource_base {
	public:
		const json_file &get_json_file() const;
		json_file &get_json_file();

	private:
		std::unique_ptr<json_file > json_file_;
		bool on_loaded(const file &file, const json_file  &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
	};
}
