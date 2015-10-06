#pragma once

#include <vector>
#include <unordered_map>
#include "ResourceBase.h"

namespace planeta_engine {
	namespace resources {
		class INIFileResource : public core::ResourceBase {
		public:
			std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::const_iterator begin()const { return _data.begin(); }
			std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::const_iterator end()const { return _data.end(); }
			std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::const_iterator find(const std::string& s)const { return _data.find(s); }
			const std::unordered_map<std::string, std::string>& GetSection(const std::string& s)const { return _data.at(s); }
		private:
			bool _Create(const std::shared_ptr<file_system::File>& file) override;
			void _Dispose()override { _data.clear(); }
			std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _data;
		};
	}
}
