#pragma once

#include "planeta/core/ResourceBase.hpp"
//#include "planeta/core/lua.hpp"

namespace plnt {
	class RLuaScript final : public resource_base {
		RLuaScript();
		~RLuaScript();
		bool RegisterToLuaState(lua_State *l);

	protected:
		virtual bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		virtual void on_disposed() override;
		std::unique_ptr<file> file_;
	};
}
