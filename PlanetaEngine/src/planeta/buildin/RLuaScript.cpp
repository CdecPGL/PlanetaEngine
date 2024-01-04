#include "planeta/core/file.hpp"

#include "RLuaScript.hpp"

namespace plnt {
	bool r_lua_script::register_to_lua_state(lua_State *l) {
		/*
		if (luaL_loadbuffer(l, reinterpret_cast<const char*>(file_->top_pointer()), file_->size(), file_->file_name().c_str())) {
			PE_LOG_ERROR("Luaステートの読み込みに失敗しました。ファイル名:", file_->file_name());
			return false;
		}
		if (lua_pcall(l, 0, LUA_MULTRET, 0)) {
			const char* err = lua_tostring(l, -1);
			PE_LOG_ERROR("Luaステートの実行に失敗しました。:", err);
			return false;
		}
		return true;
		*/
		return false;
	}

	bool r_lua_script::on_loaded(const file &f, const json_file &metadata, resource_referencer &referencer) {
		//ファイルのコピーを作成
		file_ = std::make_unique<file>(f);
		return true;
	}

	void r_lua_script::on_disposed() { file_.reset(); }

	r_lua_script::r_lua_script() = default;

	r_lua_script::~r_lua_script() = default;
}
