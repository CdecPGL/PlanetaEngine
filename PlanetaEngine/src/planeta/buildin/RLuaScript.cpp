#include "planeta/buildin/RLuaScript.hpp"
#include "planeta/File.hpp"

namespace plnt {

	bool RLuaScript::RegisterToLuaState(lua_State* l) {
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

	bool RLuaScript::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
		//ファイルのコピーを作成
		file_ = std::make_unique<File>(file);
		return true;
	}

	void RLuaScript::OnDisposed() {
		file_.release();
	}

	RLuaScript::RLuaScript() = default;

	RLuaScript::~RLuaScript() = default;
}