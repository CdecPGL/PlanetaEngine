#pragma once

#include <vector>
#include "ResourceBase.h"

namespace planeta {
	class RCsv : public private_::ResourceBase {
	public:
		using CSV_DATA = std::vector < std::vector<std::string> >;
		CSV_DATA::iterator begin();
		CSV_DATA::const_iterator cbegin()const;
		CSV_DATA::iterator end();
		CSV_DATA::const_iterator cend()const;
	private:
		std::vector<std::vector<std::string>> loaded_datas;

		bool _Create(const std::shared_ptr<const File>& file) override;
		void _Dispose()override { loaded_datas.clear(); };
		bool _CreateCSVFromLines(std::vector<std::string>&);
	};
}
