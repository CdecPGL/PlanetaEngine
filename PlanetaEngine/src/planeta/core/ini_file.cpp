#include <algorithm>

#include "ini_file.hpp"
#include "file_istream.hpp"
#include "file_system_utility.hpp"
#include "character_code.hpp"

namespace plnt {
	bool ini_file::load(const file &file) {
		using namespace std;
		file_istream fis(file);
		vector<string> lines;
		string buf;
		while (getline(fis, buf)) {
			//コメント除去
			if (auto pos = buf.find_first_of(';'); pos != std::string::npos) { buf.erase(pos, buf.size() - pos); }
			//改行コード除去
			util::remove_line_feed_code(buf);
			//スペースとタブ除去
			util::remove_space_and_tab(buf);
			//空行でなかったら行リストに追加
			if (!buf.empty()) { lines.push_back(std::move(buf)); }
		}

		//セクション分離
		unordered_map<string, vector<string>> section;
		auto s_it = ranges::find_if(lines, [](const string &l) -> bool {
			if (l.size() < 3)return false;
			if (*l.begin() == '[' && *(l.end() - 1) == ']')return true;
			return false;
		});
		auto e_it = s_it;
		while (e_it != lines.end()) {
			e_it = find_if(s_it + 1, lines.end(), [](const string &l) -> bool {
				if (l.size() < 3)return false;
				if (*l.begin() == '[' && *(l.end() - 1) == ']')return true;
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
		ini_type origin_data;
		for (auto it = section.begin(); it != section.end(); ++it) {
			unordered_map<string, string> s;
			for (string &l : (*it).second) {
				auto eq_idx = l.find('=');
				if (eq_idx == std::string::npos) { continue; }
				string name = l.substr(0, eq_idx);
				string value = l.substr(eq_idx + 1, l.size() - 1);
				s.insert(pair(name, value));
			}
			origin_data.insert(pair((*it).first, s));
		}

		//UTF8からシステム文字コードへ変換
		for (auto &[section_name, section_content] : origin_data) {
			unordered_map<string, string> n_sec;
			for (auto &[key, value] : section_content) {
				n_sec.emplace(util::convert_utf8_to_system_code(key),
				              util::convert_utf8_to_system_code(value));
			}
			data_.emplace(util::convert_utf8_to_system_code(section_name), std::move(n_sec));
		}
		return true;
	}

	boost::optional<const ini_file::section_type &> ini_file::get_section(const std::string &s) const {
		try { return data_.at(s); } catch (std::out_of_range &) { return boost::none; }
	}
}
