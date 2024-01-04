#pragma once

#include "../core/resource_base.hpp"
//#include "planeta/core/lua.hpp"

namespace plnt {
	class r_lua_script final : public resource_base {
		r_lua_script();
		~r_lua_script() override;
		bool register_to_lua_state(lua_State *l);

	protected:
		bool on_loaded(const file &f, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		std::unique_ptr<file> file_;
	};
}
