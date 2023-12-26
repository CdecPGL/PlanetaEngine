#include<fstream>
#include<algorithm>

#include "ini_loader.hpp"

using namespace std;

int ini_loader::load_ini(const string &filename) {
	ifstream ifs(filename);
	if (ifs.fail()) {
		printf("INILoader::LoadINI \"%s\"を開けませんでした。\n", filename.c_str());
		return -1;
	}
	vector<string> lines;
	string buf;
	while (getline(ifs, buf)) { lines.push_back(buf); }
	ifs.close();

	//コメント行,空行を除去
	erase_if(lines, [](const string &l)-> bool {
		if (l.empty()) { return true; }
		if (l[0] == ';') { return true; }
		return false;
	});

	//セクション分離
	unordered_map<string, vector<string>> section;
	auto s_it = ranges::find_if(lines, [](const string &l) -> bool {
		if (l.size() < 3)return false;
		if (*(l.begin()) == '[' && *(l.end() - 1) == ']')return true;
		return false;
	});
	auto e_it = s_it;
	while (e_it != lines.end()) {
		e_it = find_if(s_it + 1, lines.end(), [](const string &l) -> bool {
			if (l.size() < 3)return false;
			if (*(l.begin()) == '[' && *(l.end() - 1) == ']')return true;
			return false;
		});
		string s_name = (*s_it).substr(1, (*s_it).size() - 2);
		vector<string> s_in;
		s_in.resize(e_it - s_it - 1);
		copy(s_it + 1, e_it, s_in.begin());
		section.insert(pair(s_name, s_in));
		s_it = e_it;
	}

	//データ抽出
	for (auto it = section.begin(); it != section.end(); ++it) {
		unordered_map<string, string> s;
		for (string &l : (*it).second) {
			//スペース除去
			erase_if(l, [](const char w)-> bool { return w == ' '; });
			auto eq_idx = l.find('=');
			if (eq_idx == std::string::npos) { continue; }
			string name = l.substr(0, eq_idx);
			string value = l.substr(eq_idx + 1, l.size() - 1);
			s.insert(pair(name, value));
		}
		//data.insert(pair<string, unordered_map<string, string>>((*it).first,s));
		data_.set_section((*it).first, std::move(s));
	}
	return 0;
}

//[]
unordered_map<std::string, std::string> ini_loader::operator[](const std::string &idx) const {
	const auto it = data_.find(idx);
	if (it == data_.end()) { return {}; }
	return (*it).second;
}

//セクション取得
unordered_map<std::string, std::string> ini_loader::get_section(const std::string &idx) const {
	const auto it = data_.find(idx);
	if (it == data_.end()) { return {}; }
	return (*it).second;
}

//セクション存在確認
bool ini_loader::check_section_existence(const std::string &idx) const { return !(data_.find(idx) == data_.end()); }

//値取得
string ini_loader::get_value(const string &sec, const string &name) const {
	const auto it = data_.find(sec);
	if (it == data_.end()) { return "section not found"; }
	const auto it2 = (*it).second.find(name);
	if (it2 == (*it).second.end()) { return "value no found"; }
	return (*it2).second;
}
