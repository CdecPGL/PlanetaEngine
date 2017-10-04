#pragma once

#include "ResourceBase.h"
//#include "lua.hpp"

namespace planeta {
	class RLuaScript final: public ResourceBase {
		RLuaScript();
		~RLuaScript();
		bool RegisterToLuaState(lua_State* l);
	protected:
		virtual bool _Create(const File& file, ResourceReferencer&) override;
		virtual void _Dispose() override;
		std::unique_ptr<File> file_;
	};
}
