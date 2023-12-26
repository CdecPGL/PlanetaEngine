#pragma once

#include "planeta/core/ResourceBase.hpp"
//#include "planeta/core/lua.hpp"

namespace plnt {
	class RLuaScript final : public ResourceBase {
		RLuaScript();
		~RLuaScript();
		bool RegisterToLuaState(lua_State *l);

	protected:
		virtual bool OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) override;
		virtual void OnDisposed() override;
		std::unique_ptr<File> file_;
	};
}
