#pragma once

#include "planeta/core/ResourceBase.hpp"

namespace plnt {
	class RText : public ResourceBase {
	public:
		RText() {};
		~RText() {};
		const std::string& GetText()const { return _text; }
	private:
		virtual bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		virtual void OnDisposed() override;
		std::string _text;
	};
}
