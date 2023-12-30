#pragma once

#include "planeta/core/ResourceBase.hpp"

namespace plnt {
	class RText : public resource_base {
	public:
		RText() { };

		~RText() { };
		const std::string &GetText() const { return _text; }

	private:
		virtual bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		virtual void on_disposed() override;
		std::string _text;
	};
}
