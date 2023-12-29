#pragma once

#include "planeta/core/ResourceBase.hpp"
//#include "planeta/core/lua.hpp"

namespace plnt {
	class RLuaScript final : public ResourceBase {
		RLuaScript();
		~RLuaScript();
		bool RegisterToLuaState(lua_State *l);

	protected:
		virtual bool OnLoaded(const file &file, const json_file &metadata, ResourceReferencer &referencer) override;
		virtual void OnDisposed() override;
		std::unique_ptr<file> file_;
	};
}
