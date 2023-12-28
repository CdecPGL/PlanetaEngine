#pragma once

#include <unordered_map>
#include <memory>
#include <string>

namespace plnt {
	class File;

	class xml_element {
	public:
		[[nodiscard]] std::shared_ptr<const xml_element> find_first_child(const std::string &name) const;
		void add_child(const std::string &name, const std::shared_ptr<xml_element> &elem);
		[[nodiscard]] const std::string &name() const { return name_; }
		void name(const std::string &n);
		[[nodiscard]] const std::string &text() const { return text_; }
		void text(const std::string &t) { text_ = t; }

	private:
		std::string name_;
		std::string text_;
		std::unordered_multimap<std::string, std::shared_ptr<xml_element>> children_;
		xml_element *parent_ = nullptr;
	};

	/*! XMLファイルクラス*/
	class xml_file final {
	public:
		[[nodiscard]] std::shared_ptr<const xml_element> get_root_element() const { return root_element_; }
		bool load(const File &file);

	private:
		std::shared_ptr<xml_element> root_element_;
	};
}
