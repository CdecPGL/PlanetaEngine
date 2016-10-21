#pragma once

//namespace planeta {
//public:
//	using SectionType = std::unordered_map<std::string, std::string>;
//	using INIType = std::unordered_map<std::string, SectionType>;
//	INIType::const_iterator begin()const { return _data.begin(); }
//	INIType::const_iterator end()const { return _data.end(); }
//	INIType::const_iterator find(const std::string& s)const { return _data.find(s); }
//	boost::optional<const SectionType&> GetSection(const std::string& s)const;
//private:
//	bool _Create(const File& file) override;
//	void _Dispose()override { _data.clear(); }
//	INIType _data;
//};
//}