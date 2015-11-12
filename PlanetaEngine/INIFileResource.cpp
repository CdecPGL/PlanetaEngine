#include "INIFileResource.h"

namespace planeta_engine {
	namespace resources {

		bool INIFileResource::_Create(const std::shared_ptr<const file_system::File>& file)
		{
		//	using namespace std;
		//	ifstream ifs(filename);
		//	if (ifs.fail()) {
		//		printf("INILoader::LoadINI \"%s\"を開けませんでした。\n", filename.c_str());
		//		return -1;
		//	}
		//	vector<string> lines;
		//	string buf;
		//	while (getline(ifs, buf)) {
		//		lines.push_back(move(buf));
		//	}
		//	ifs.close();

		//	//コメント行,空行を除去
		//	lines.erase(remove_if(lines.begin(), lines.end(), [](string& l)->bool {
		//		if (l.size() == 0) { return true; }
		//		if (l[0] == ';') { return true; }
		//		return false;
		//	}), lines.end());

		//	//セクション分離
		//	unordered_map<string, vector<string>> section;
		//	vector<string>::iterator s_it = find_if(lines.begin(), lines.end(), [](string& l) -> bool {
		//		if (l.size() < 3)return false;
		//		if (*(l.begin()) == '[' && *(l.end() - 1) == ']')return true;
		//		return false;
		//	});
		//	vector<string>::iterator e_it = s_it;
		//	while (e_it != lines.end()) {
		//		e_it = find_if(s_it + 1, lines.end(), [](string& l) -> bool {
		//			if (l.size() < 3)return false;
		//			if (*(l.begin()) == '[' && *(l.end() - 1) == ']')return true;
		//			return false;
		//		});
		//		string s_name = (*s_it).substr(1, (*s_it).size() - 2);
		//		vector<string> s_in;
		//		s_in.resize(e_it - s_it - 1);
		//		copy(s_it + 1, e_it, s_in.begin());
		//		section.insert(pair<string, vector<string>>(s_name, s_in));
		//		s_it = e_it;
		//	}

		//	//データ抽出
		//	for (auto it = section.begin(); it != section.end(); ++it) {
		//		unordered_map<string, string> s;
		//		for (string& l : (*it).second) {
		//			//スペース除去
		//			l.erase(remove_if(l.begin(), l.end(), [](char w)->bool { return w == ' '; }), l.end());
		//			int eq_idx = l.find('=');
		//			if (eq_idx == -1) { continue; }
		//			string name = l.substr(0, eq_idx);
		//			string value = l.substr(eq_idx + 1, l.size() - 1);
		//			s.insert(pair<string, string>(name, value));
		//		}
		//		//data.insert(pair<string, unordered_map<string, string>>((*it).first,s));
		//		_data.SetSection((*it).first, std::move(s));
		//	}
		//	return 0;
			return false;
		}

	}
}