#pragma once

#include <unordered_map>
#include <memory>
#include "ResourceBase.h"

namespace planeta {
	class XMLElement :public Object {
	public:
		std::shared_ptr<const XMLElement> FindFirstChild(const std::string& name)const;
		void AddChild(const std::string& name, const std::shared_ptr<XMLElement>& elem);
		const std::string& name()const { return name_; }
		void name(const std::string& n);
		const std::string& text()const { return text_; }
		void text(const std::string& t) { text_ = t; }
	private:
		std::string name_;
		std::string text_;
		std::unordered_multimap<std::string, std::shared_ptr<XMLElement>> children_;
		XMLElement* parent_ = nullptr;
	};

	class RXml final : public private_::ResourceBase {
	public:
		std::shared_ptr<const XMLElement> GetRootElement()const { return root_element_; }
	private:
		bool _Create(const File& file) override;
		void _Dispose() override;
		std::shared_ptr<XMLElement> root_element_;
	};
}