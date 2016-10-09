#pragma once

#include "ResourceBase.h"
#include "lua.hpp"

namespace planeta {
	class RLuaScript final: public private_::ResourceBase {
		RLuaScript();
		~RLuaScript();
		bool RegisterToLuaState(lua_State* l);
	protected:
		virtual bool _Create(const File& file) override;
		virtual void _Dispose() override;
		std::unique_ptr<File> file_;
	};
}
