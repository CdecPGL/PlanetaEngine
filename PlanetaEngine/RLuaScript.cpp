#include "RLuaScript.h"

namespace planeta {

	bool RLuaScript::RegisterToLuaState(lua_State* l) {
		/*if (luaL_loadbuffer(l, )) {
			fprintf(stderr, "cannot open %s\n", fname);
			return;
		}*/
		return true;
	}

	bool RLuaScript::_Create(const File& file) {
		throw std::logic_error("The method or operation is not implemented.");
	}

	void RLuaScript::_Dispose() {
		throw std::logic_error("The method or operation is not implemented.");
	}

}