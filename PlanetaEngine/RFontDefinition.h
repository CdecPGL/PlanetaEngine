#pragma once

#include "ResourceBase.h"

namespace planeta {
	class RFontDefinition final : public private_::ResourceBase {
	public:
		int GetHandle()const { return handle_; }
		int font_size()const { return size_; }
	private:
		bool _Create(const File& file, private_::ResourceReferencer&) override;
		void _Dispose() override;
		int handle_;
		int size_ = 0;
	};
}