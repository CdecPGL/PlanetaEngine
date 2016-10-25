﻿#pragma once
#include "ResourceBase.h"

namespace planeta {
	class RText : public ResourceBase {
	public:
		RText() {};
		~RText() {};
		const std::string& GetText()const { return _text; }
	private:
		virtual bool _Create(const File& file, ResourceReferencer&) override;
		virtual void _Dispose() override;
		std::string _text;
	};
}
