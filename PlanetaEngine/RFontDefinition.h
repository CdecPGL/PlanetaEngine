#pragma once

#include "ResourceBase.h"

namespace planeta {
	class RFontDefinition final : public ResourceBase {
	public:
		int GetHandle()const { return handle_; }
		int font_size()const { return size_; }
	private:
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
		int handle_;
		int size_ = 0;
	};
}