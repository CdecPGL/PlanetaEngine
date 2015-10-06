//boost::splitに関するC4996エラーをこのファイルのみ無効化
#pragma warning(push) 
#pragma warning(disable:4996) 

#include "CSVResource.h"

#include <algorithm>
#include "boost/algorithm/string.hpp"
#include "File.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace resources {

		bool CSVResource::_Create(const std::shared_ptr<file_system::File>& file) {
			if (file->GetStatus() != file_system::File::FileStatus::Available) { return false; }
			size_t size = file->GetSize();
			const char* c_ptr = (const char*)file->GetTopPointer();
			unsigned int pos = 0;
			std::vector<std::string> lines;
			std::string buf;
			while (pos < size) {
				char w = c_ptr[pos];
				if (w == '\r') {
				}
				else if (w == '\n') {
					lines.push_back(move(buf));
					buf.clear();
				}
				else {
					buf.push_back(w);
				}
				++pos;
			}
			//	buf.pop_back();
			if (buf.size() != 0) { lines.push_back(move(buf)); }
			return _CreateCSVFromLines(lines);
		}

		bool CSVResource::_CreateCSVFromLines(std::vector<std::string>& lines) {
			loaded_datas.clear();
			using namespace boost;
			//無効な行を取り除く(空だったら)
			std::remove_if(lines.begin(), lines.end(), [](std::string line)->bool {
				if (line.size() == 0)return true;
				return false;
			});
			//行がなかったら警告
			if (lines.size() == 0) {
				char str[256];
				sprintf_s(str, 256, "空のSCVファイルです。又は行の抽出に失敗しました。");
				debug::SystemLog::instance().LogWarning(str, "CSVResource::_CreateCSVFromLines");
				return true;
			}
			//データ抽出
			int i = 1;
			for (std::string& line : lines) {
				std::vector<std::string> datas;
				algorithm::split(datas, line, algorithm::is_any_of(","));
				if (datas.size() == 0) {
					char str[256];
					sprintf_s(str, 256, "csvファイルの読み込みエラー。:データの抽出に失敗しました(%d行目)。", i);
					debug::SystemLog::instance().LogError(str, "CSVLoader::CreateCSVFromLines");
					return false;
				}
				++i;
				//\nを取り除く
				for (std::string& data : datas) {
					std::remove_if(data.begin(), data.end(), [](char w)->bool {return w == '/n'; });
				}
				//データを保存
				loaded_datas.push_back(std::move(datas));
			}
			return true;
		}

		CSVResource::CSV_DATA::iterator CSVResource::begin() { return loaded_datas.begin(); }
		CSVResource::CSV_DATA::const_iterator CSVResource::cbegin()const { return loaded_datas.cbegin(); }

		CSVResource::CSV_DATA::iterator CSVResource::end() { return loaded_datas.end(); }
		CSVResource::CSV_DATA::const_iterator CSVResource::cend()const { return loaded_datas.cend(); }
	}
}

#pragma warning(pop)