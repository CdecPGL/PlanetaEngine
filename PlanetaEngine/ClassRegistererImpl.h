#pragma once

#include "ClassRegisterer.h"

namespace planeta {
	template<class C>
	class ClassRegistererImpl : ClassRegisterer<C> {
	public:
		void RegisterToLuaStateWithLuaIntf(const std::string& object_type_id, LuaIntf::LuaBinding& bl) {
			lua_binder_([] {bl.beginClass<C>(object_type_id.c_str())}).endClass();
		}
		bool is_bind_to_script()const { return is_bind_to_script_; }
	};
}