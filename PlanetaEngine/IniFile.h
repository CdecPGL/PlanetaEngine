#pragma once

#include <vector>
#include <unordered_map>
#include "boost/optional.hpp"

namespace planeta {
	class File;
	/*! INI�t�@�C�����������߂̃N���X*/
	class IniFile final{
	public:
		using SectionType = std::unordered_map<std::string, std::string>;
		using INIType = std::unordered_map<std::string, SectionType>;
		INIType::const_iterator begin()const { return _data.begin(); }
		INIType::const_iterator end()const { return _data.end(); }
		INIType::const_iterator find(const std::string& s)const { return _data.find(s); }
		boost::optional<const SectionType&> GetSection(const std::string& s)const;
		bool Load(const File& file);
	private:
		INIType _data;
	};
}