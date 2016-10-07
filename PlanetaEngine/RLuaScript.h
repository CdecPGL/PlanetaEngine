#pragma once

#include "ResourceBase.h"
#include "lua.hpp"

namespace planeta {
	class RLuaScript : public private_::ResourceBase {
		bool RegisterToLuaState(lua_State* l);
	protected:
		virtual bool _Create(const File& file) override;
		virtual void _Dispose() override;
	};
}
