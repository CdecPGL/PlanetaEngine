//boost::split�Ɋւ���C4996�G���[�����̃t�@�C���̂ݖ�����
#pragma warning(push) 
#pragma warning(disable:4996) 

#include "CsvFile.h"

#include <algorithm>
#include "boost/algorithm/string.hpp"
#include "File.h"
#include "SystemLog.h"
#include "CharacterCode.h"


namespace planeta {
	bool CsvFile::Load(const File& file) {
		size_t size = file.size();
		const char* c_ptr = (const char*)file.top_pointer();
		unsigned int pos = 0;
		std::vector<std::string> lines;
		std::string buf;
		while (pos < size) {
			char w = c_ptr[pos];
			if (w == '\r') {
			} else if (w == '\n') {
				lines.push_back(move(buf));
				buf.clear();
			} else {
				buf.push_back(w);
			}
			++pos;
		}
		//	buf.pop_back();
		if (buf.size() != 0) { lines.push_back(move(buf)); }
		return _CreateCSVFromLines(lines);
	}

	bool CsvFile::_CreateCSVFromLines(std::vector<std::string>& lines) {
		loaded_datas.clear();
		using namespace boost;
		//�����ȍs����菜��(�󂾂�����)
		std::remove_if(lines.begin(), lines.end(), [](std::string line)->bool {
			if (line.size() == 0)return true;
			return false;
		});
		//�s���Ȃ�������x��
		if (lines.size() == 0) {
			PE_LOG_WARNING("���SCV�t�@�C���ł��B���͍s�̒��o�Ɏ��s���܂����B");
			return true;
		}
		//�f�[�^���o
		int i = 1;
		for (std::string& line : lines) {
			std::vector<std::string> datas;
			algorithm::split(datas, line, algorithm::is_any_of(","));
			if (datas.size() == 0) {
				PE_LOG_ERROR("csv�t�@�C���̓ǂݍ��݃G���[�B:�f�[�^�̒��o�Ɏ��s���܂���(", i, "�s��)�B");
				return false;
			}
			++i;
			//\n����菜��
			for (std::string& data : datas) {
				std::remove_if(data.begin(), data.end(), [](char w)->bool {return w == '/n'; });
			}
			//�f�[�^��ۑ�
			loaded_datas.push_back(std::move(datas));
		}

		//�ǂݍ��񂾃f�[�^�̕����R�[�h��UTF8����V�X�e�������R�[�h�֕ϊ�
		for (auto&& line : loaded_datas) {
			for (auto&& elem : line) {
				elem = util::ConvertUTF8ToSystemCode(elem);
			}
		}
		return true;
	}

	CsvFile::CSV_DATA::iterator CsvFile::begin() { return loaded_datas.begin(); }
	CsvFile::CSV_DATA::const_iterator CsvFile::cbegin()const { return loaded_datas.cbegin(); }

	CsvFile::CSV_DATA::iterator CsvFile::end() { return loaded_datas.end(); }
	CsvFile::CSV_DATA::const_iterator CsvFile::cend()const { return loaded_datas.cend(); }
}

#pragma warning(pop)