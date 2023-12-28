#include <algorithm>

#include "IniFile.hpp"
#include "file_istream.hpp"
#include "file_system_utility.hpp"
#include "character_code.hpp"

namespace plnt {
	bool IniFile::Load(const file &file) {
		using namespace std;
		file_istream fis(file);
		vector<string> lines;
		string buf;
		while (getline(fis, buf)) {
			//コメント除去
			auto pos = buf.find_first_of(';');
			if (pos != std::string::npos) { buf.erase(pos, buf.size() - pos); }
			//改行コード除去
			util::remove_line_feed_code(buf);
			//スペースとタブ除去
			util::remove_space_and_tab(buf);
			//空行でなかったら行リストに追加
			if (buf.size() != 0) { lines.push_back(move(buf)); }
		}

		//セクション分離
		unordered_map<string, vector<string>> section;
		vector<string>::iterator s_it = find_if(lines.begin(), lines.end(), [](string &l) -> bool {
			if (l.size() < 3)return false;
			if (*(l.begin()) == '[' && *(l.end() - 1) == ']')return true;
			return false;
		});
		vector<string>::iterator e_it = s_it;
		while (e_it != lines.end()) {
			e_it = find_if(s_it + 1, lines.end(), [](string &l) -> bool {
				if (l.size() < 3)return false;
				if (*(l.begin()) == '[' && *(l.end() - 1) == ']')return true;
				return false;
			});
			string s_name = (*s_it).substr(1, (*s_it).size() - 2);
			vector<string> s_in;
			s_in.resize(e_it - s_it - 1);
			copy(s_it + 1, e_it, s_in.begin());
			section.insert(pair<string, vector<string>>(s_name, s_in));
			s_it = e_it;
		}

		//データ抽出
		INIType origin_data;
		for (auto it = section.begin(); it != section.end(); ++it) {
			unordered_map<string, string> s;
			for (string &l : (*it).second) {
				auto eq_idx = l.find('=');
				if (eq_idx == std::string::npos) { continue; }
				string name = l.substr(0, eq_idx);
				string value = l.substr(eq_idx + 1, l.size() - 1);
				s.insert(pair<string, string>(name, value));
			}
			origin_data.insert(pair<string, unordered_map<string, string>>((*it).first, s));
		}

		//UTF8からシステム文字コードへ変換
		for (auto &&sec : origin_data) {
			unordered_map<string, string> n_sec;
			for (auto &&data : sec.second) {
				n_sec.emplace(util::convert_utf8_to_system_code(data.first), util::convert_utf8_to_system_code(data.second));
			}
			_data.emplace(util::convert_utf8_to_system_code(sec.first), std::move(n_sec));
		}
		return true;
	}

	boost::optional<const IniFile::SectionType &> IniFile::GetSection(const std::string &s) const {
		try { return _data.at(s); } catch (std::out_of_range &) { return boost::none; }
	}
}
