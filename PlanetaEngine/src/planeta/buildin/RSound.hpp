#pragma once

#include "..\core\resource_base.hpp"

namespace plnt {
	/*効果音リソース*/
	class RSound : public resource_base {
	public:
		RSound() : _handle(-1) { };

		~RSound() { };
		int GetHandle() const { return _handle; }
		int GetTotalTimeByMilliSecond();

	private:
		int _handle;
		virtual bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		virtual void on_disposed() override;
	};
}
