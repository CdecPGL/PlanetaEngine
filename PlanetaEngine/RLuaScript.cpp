#include "RLuaScript.h"
#include "file.h"

namespace planeta {

	bool RLuaScript::RegisterToLuaState(lua_State* l) {
		if (luaL_loadbuffer(l, reinterpret_cast<const char*>(file_->top_pointer()), file_->size(), file_->file_name().c_str())) {
			return false;
		}
		return true;
	}

	bool RLuaScript::_Create(const File& file) {
		file_ = std::make_unique<File>(file);
		return true;
	}

	void RLuaScript::_Dispose() {
		file_.release();
	}

	RLuaScript::RLuaScript() = default;

	RLuaScript::~RLuaScript() = default;
}