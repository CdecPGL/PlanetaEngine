//boost::split�Ɋւ���C4996�G���[�����̃t�@�C���̂ݖ�����
#pragma warning(push) 
#pragma warning(disable:4996) 

#include "RCsv.h"

#include <algorithm>
#include "boost/algorithm/string.hpp"
#include "File.h"
#include "SystemLog.h"
#include "CharacterCode.h"

namespace planeta {
	bool RCsv::_Create(const std::shared_ptr<const File>& file) {
		if (file->GetStatus() != File::FileStatus::Available) { return false; }
		size_t size = file->GetSize();
		const char* c_ptr = (const char*)file->GetTopPointer();
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

	bool RCsv::_CreateCSVFromLines(std::vector<std::string>& lines) {
		loaded_datas.clear();
		using namespace boost;
		//�����ȍs����菜��(�󂾂�����)
		std::remove_if(lines.begin(), lines.end(), [](std::string line)->bool {
			if (line.size() == 0)return true;
			return false;
		});
		//�s���Ȃ�������x��
		if (lines.size() == 0) {
			char str[256];
			sprintf_s(str, 256, "���SCV�t�@�C���ł��B���͍s�̒��o�Ɏ��s���܂����B");
			debug::SystemLog::instance().LogWarning(str, "CSVResource::_CreateCSVFromLines");
			return true;
		}
		//�f�[�^���o
		int i = 1;
		for (std::string& line : lines) {
			std::vector<std::string> datas;
			algorithm::split(datas, line, algorithm::is_any_of(","));
			if (datas.size() == 0) {
				char str[256];
				sprintf_s(str, 256, "csv�t�@�C���̓ǂݍ��݃G���[�B:�f�[�^�̒��o�Ɏ��s���܂���(%d�s��)�B", i);
				debug::SystemLog::instance().LogError(str, "CSVLoader::CreateCSVFromLines");
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

	RCsv::CSV_DATA::iterator RCsv::begin() { return loaded_datas.begin(); }
	RCsv::CSV_DATA::const_iterator RCsv::cbegin()const { return loaded_datas.cbegin(); }

	RCsv::CSV_DATA::iterator RCsv::end() { return loaded_datas.end(); }
	RCsv::CSV_DATA::const_iterator RCsv::cend()const { return loaded_datas.cend(); }
}

#pragma warning(pop)