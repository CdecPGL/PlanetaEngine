#pragma once

#include "class_registerer.hpp"
#include "reflection_utility.hpp"

namespace plnt::reflection::private_ {
	//superエイリアスが指定されていないクラスの場合
	template <class C, typename T = void>
	class class_registerer_impl : public class_registerer<C> {
	public:
		using class_registerer<C>::class_registerer;

		void begin_proc() { }

		void end_proc() {
			//LuaBinderの設定
			#ifndef DISABLE_SCRIPT_REGISTRATION
				class_info_.lua_binder = [lb = lua_binder_core_, class_id = class_info_.object_type_id](lua_State* L) {
					lb([L, &class_id]() {return LuaIntf::LuaBinding(L).beginClass<C>(class_id.c_str()); }).endClass();
				};
			#endif
		}
	};

	//superエイリアスが指定されているクラスの場合
	template <class C>
	class class_registerer_impl<C, boost::enable_if<has_super_alias<C>, void>> : class_registerer<C> {
	public:
		using class_registerer<C>::class_registerer;

		void begin_proc() { }

		void end_proc() {
			//LuaBinderの設定
			#ifndef DISABLE_SCRIPT_REGISTRATION
				class_info_.lua_binder = [lb = lua_binder_core_, class_id = class_info_.object_type_id](lua_State* L) {
					lb([L, &class_id]() {return LuaIntf::LuaBinding(L).beginExtendClass<C, typename C::super>(class_id.c_str()); }).endClass();
				};
			#endif
		}
	};
}
