#pragma once
#include "ResourceBase.h"

namespace planeta {
	class TextResource : public core::ResourceBase {
	public:
		TextResource() {};
		~TextResource() {};
		const std::string& GetText()const { return _text; }
	private:
		virtual bool _Create(const std::shared_ptr<const File>& file) override;
		virtual void _Dispose() override;
		std::string _text;
	};
}
