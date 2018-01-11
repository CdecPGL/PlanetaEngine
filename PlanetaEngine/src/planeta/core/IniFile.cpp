#include <algorithm>

#include "IniFile.hpp"
#include "FileIStream.hpp"
#include "FileSystemUtility.hpp"
#include "CharacterCode.hpp"

namespace plnt {
	bool IniFile::Load(const File& file) {
		using namespace std;
		FileIStream fis(file);
		vector<string> lines;
		string buf;
		while (getline(fis, buf)) {
			//�R�����g����
			auto pos = buf.find_first_of(';');
			if (pos != std::string::npos) {
				buf.erase(pos, buf.size() - pos);
			}
			//���s�R�[�h����
			util::RemoveLineFeedCode(buf);
			//�X�y�[�X�ƃ^�u����
			util::RemoveSpaceAndTab(buf);
			//��s�łȂ�������s���X�g�ɒǉ�
			if (buf.size() != 0) {
				lines.push_back(move(buf));
			}
		}

		//�Z�N�V��������
		unordered_map<string, vector<string>> section;
		vector<string>::iterator s_it = find_if(lines.begin(), lines.end(), [](string& l) -> bool {
			if (l.size() < 3)return false;
			if (*(l.begin()) == '[' && *(l.end() - 1) == ']')return true;
			return false;
		});
		vector<string>::iterator e_it = s_it;
		while (e_it != lines.end()) {
			e_it = find_if(s_it + 1, lines.end(), [](string& l) -> bool {
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

		//�f�[�^���o
		INIType origin_data;
		for (auto it = section.begin(); it != section.end(); ++it) {
			unordered_map<string, string> s;
			for (string& l : (*it).second) {
				int eq_idx = l.find('=');
				if (eq_idx == -1) { continue; }
				string name = l.substr(0, eq_idx);
				string value = l.substr(eq_idx + 1, l.size() - 1);
				s.insert(pair<string, string>(name, value));
			}
			origin_data.insert(pair<string, unordered_map<string, string>>((*it).first, s));
		}

		//UTF8����V�X�e�������R�[�h�֕ϊ�
		for (auto&& sec : origin_data) {
			unordered_map<string, string> n_sec;
			for (auto&& data : sec.second) {
				n_sec.emplace(util::ConvertUTF8ToSystemCode(data.first), util::ConvertUTF8ToSystemCode(data.second));
			}
			_data.emplace(util::ConvertUTF8ToSystemCode(sec.first), std::move(n_sec));
		}
		return true;
	}

	boost::optional<const IniFile::SectionType&> IniFile::GetSection(const std::string& s) const {
		try {
			return _data.at(s);
		} catch (std::out_of_range&) {
			return boost::none;
		}
	}
}