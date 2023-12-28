#include <cassert>
#include <functional>

#include "xml_file.hpp"
#include "File.hpp"
#include "tinyxml2.h"
#include "LogUtility.hpp"
#include "character_code.hpp"

namespace plnt {
	bool xml_file::load(const File &file) {
		std::function<void(const std::shared_ptr<xml_element> &, const tinyxml2::XMLNode &)> element_setter = [&
				element_setter](const std::shared_ptr<xml_element> &target, const tinyxml2::XMLNode &source) {
			for (auto *s_element = source.FirstChildElement(); s_element != nullptr; s_element = s_element->
			     NextSiblingElement()) {
				auto c_element = std::make_shared<xml_element>();
				target->add_child(util::convert_utf8_to_system_code(s_element->Name()), c_element);
				c_element->text(s_element->GetText() ? util::convert_utf8_to_system_code(s_element->GetText()) : "");
				element_setter(c_element, *s_element);
			}
		};
		root_element_ = std::make_shared<xml_element>();
		tinyxml2::XMLDocument xml_document;
		if (xml_document.Parse(reinterpret_cast<const char *>(file.top_pointer()), file.size()) ==
			tinyxml2::XMLError::XML_SUCCESS) {
			element_setter(root_element_, xml_document);
			return true;
		}
		PE_LOG_ERROR("XMLファイルの解析に失敗しました。(", xml_document.ErrorName(), "(", xml_document.ErrorID(), "), 行番号: ",
		             xml_document.ErrorLineNum(), "詳細: ", xml_document.ErrorStr(), ")");
		return false;
	}

	std::shared_ptr<const xml_element> xml_element::find_first_child(const std::string &name) const {
		const auto it = children_.find(name);
		return it == children_.end() ? nullptr : it->second;
	}

	void xml_element::name(const std::string &n) {
		name_ = n;
		if (parent_) {
			using map_type_it = std::unordered_multimap<std::string, std::shared_ptr<xml_element>>::iterator;
			map_type_it it;
			const std::pair<map_type_it, map_type_it> range = parent_->children_.equal_range(name_);
			for (it = range.first; it != range.second; ++it) { if (it->second.get() == this) { break; } }
			assert(it != range.second);
			auto me = it->second;
			parent_->children_.erase(it);
			parent_->children_.emplace(name_, me);
		}
	}

	void xml_element::add_child(const std::string &name, const std::shared_ptr<xml_element> &elem) {
		elem->parent_ = this;
		elem->name_ = name;
		children_.emplace(name, elem);
	}
}
