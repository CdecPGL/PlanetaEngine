#pragma once

#include <memory>
#include <string>

namespace plnt {
	class file;

	namespace util {
		/*標準ファイル入力からFileを作成する*/
		std::shared_ptr<file> create_file_from_standard_file_input(const std::string &file_name);
		/*改行コードを\nに統一する(\r\nや\rを\nに)*/
		void unify_line_feed_code(std::string &str);
		/*改行コードを取り除く(\rと\n)*/
		void remove_line_feed_code(std::string &str);
		/*スペースとタブを取り除く*/
		void remove_space_and_tab(std::string &str);
	}
}
