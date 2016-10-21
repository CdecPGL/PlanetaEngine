#include "XmlFile.h"

#include <cassert>
#include <functional>
#include "File.h"
#include "tinyxml2.h"
#include "SystemLog.h"
#include "CharacterCode.h"

namespace planeta {
	bool XmlFile::Load(const File& file) {
		std::function<void(const std::shared_ptr<XMLElement>&, const tinyxml2::XMLNode&)> element_setter = [&element_setter](const std::shared_ptr<XMLElement>& target, const tinyxml2::XMLNode& source) {
			for (auto* s_element = source.FirstChildElement(); s_element != nullptr; s_element = s_element->NextSiblingElement()) {
				auto c_element = std::make_shared<XMLElement>();
				target->AddChild(util::ConvertUTF8ToSystemCode(s_element->Name()), c_element);
				c_element->text(s_element->GetText() ? util::ConvertUTF8ToSystemCode(s_element->GetText()) : "");
				element_setter(c_element, *s_element);
			}
		};
		root_element_ = std::make_shared<XMLElement>();
		tinyxml2::XMLDocument xml_document;
		if (xml_document.Parse(reinterpret_cast<const char*>(file.top_pointer()), file.size()) == tinyxml2::XMLError::XML_SUCCESS) {
			element_setter(root_element_, xml_document);
			return true;
		} else { //失敗
			const char* err1 = xml_document.GetErrorStr1();
			const char* err2 = xml_document.GetErrorStr2();
			std::string err1_str = err1 ? err1 : "不明";
			std::string err2_str = err2 ? err2 : "不明";
			PE_LOG_ERROR("XMLファイルの解析に失敗しました。(エラー詳細1[", err1_str, "],2[", err2_str, "])");
			return false;
		}
	}

	std::shared_ptr<const XMLElement> XMLElement::FindFirstChild(const std::string& name)const {
		auto it = children_.find(name);
		return it == children_.end() ? nullptr : it->second;
	}

	void XMLElement::name(const std::string& n) {
		name_ = n;
		if (parent_) {
			using cmap_type_it = std::unordered_multimap<std::string, std::shared_ptr<XMLElement>>::iterator;
			cmap_type_it it;
			std::pair<cmap_type_it, cmap_type_it> range = parent_->children_.equal_range(name_);
			for (it = range.first; it != range.second; ++it) {
				if (it->second.get() == this) { break; }
			}
			assert(it != range.second);
			auto me = it->second;
			parent_->children_.erase(it);
			parent_->children_.emplace(name_, me);
		}
	}

	void XMLElement::AddChild(const std::string& name, const std::shared_ptr<XMLElement>& elem) {
		elem->parent_ = this;
		elem->name_ = name;
		children_.emplace(name, elem);
	}
}