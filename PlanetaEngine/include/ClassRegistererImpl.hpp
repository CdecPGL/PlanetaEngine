#pragma once

#include "ClassRegisterer.hpp"
#include "ReflectionUtility.hpp"

namespace planeta {
	namespace private_ {
		//Superエイリアスが指定されていないクラスの場合
		template<class C, typename T = void>
		class ClassRegistererImpl : public ClassRegisterer<C> {
		public:
			using ClassRegisterer::ClassRegisterer;
			void BegineProc() {}
			void EndProc() {
				//LuaBinderの設定
#ifndef DISABLE_SCRIPT_REGISTRATION
				class_info_.lua_binder = [lb = lua_binder_core_, class_id = class_info_.object_type_id](lua_State* L) {
					lb([L, &class_id]() {return LuaIntf::LuaBinding(L).beginClass<C>(class_id.c_str()); }).endClass();
				};
#endif
			}
		};
		//Superエイリアスが指定されているクラスの場合
		template<class C>
		class ClassRegistererImpl<C, typename boost::enable_if<HasSuperAlias<C>, void>> : ClassRegisterer<C> {
		public:
			using ClassRegisterer::ClassRegisterer;
			void BegineProc() {}
			void EndProc() {
				//LuaBinderの設定
#ifndef DISABLE_SCRIPT_REGISTRATION
				class_info_.lua_binder = [lb = lua_binder_core_, class_id = class_info_.object_type_id](lua_State* L) {
					lb([L, &class_id]() {return LuaIntf::LuaBinding(L).beginExtendClass<C, typename C::Super>(class_id.c_str()); }).endClass();
				};
#endif
			}
		};
	}
}