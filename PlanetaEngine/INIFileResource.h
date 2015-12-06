#pragma once

#include <vector>
#include <unordered_map>
#include "ResourceBase.h"

namespace planeta_engine {
	namespace resources {
		class INIFileResource : public core::ResourceBase {
		public:
			using SectionType = std::unordered_map<std::string, std::string>;
			using INIType = std::unordered_map<std::string, SectionType>;
			INIType::const_iterator begin()const { return _data.begin(); }
			INIType::const_iterator end()const { return _data.end(); }
			INIType::const_iterator find(const std::string& s)const { return _data.find(s); }
			const SectionType& GetSection(const std::string& s)const { return _data.at(s); }
		private:
			bool _Create(const std::shared_ptr<const file_system::File>& file) override;
			void _Dispose()override { _data.clear(); }
			INIType _data;
		};
	}
}
