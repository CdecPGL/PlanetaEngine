#pragma once

#include <memory>
#include <string>

namespace plnt {
	class File;
	namespace util {
		/*標準ファイル入力からFileを作成する*/
		std::shared_ptr<File> CreateFileFromStandardFileInput(const std::string& file_name);
		/*改行コードを\nに統一する(\r\nや\rを\nに)*/
		void UnifyLineFeedCode(std::string& str);
		/*改行コードを取り除く(\rと\n)*/
		void RemoveLineFeedCode(std::string& str);
		/*スペースとタブを取り除く*/
		void RemoveSpaceAndTab(std::string& str);
	}
}