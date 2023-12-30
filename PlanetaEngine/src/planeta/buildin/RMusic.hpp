#pragma once

#include "..\core\resource_base.hpp"

namespace plnt {
	/*音楽リソース*/
	class RMusic : public resource_base {
	public:
		RMusic() : _handle(-1) { };

		~RMusic() { }

		int GetHandle() const { return _handle; }
		int GetTotalTimeByMilliSecond();

	private:
		int _handle;
		virtual bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		virtual void on_disposed() override;
	};
}
