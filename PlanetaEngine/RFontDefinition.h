#pragma once

#include "ResourceBase.h"

namespace planeta {
	class RFontDefinition final : public core::ResourceBase {
	public:
		int GetHandle()const { return handle_; }
		int font_size()const { return size_; }
	private:
		bool _Create(const std::shared_ptr<const File>& file) override;
		void _Dispose() override;
		int handle_ = -1;
		int size_ = 0;
	};
}