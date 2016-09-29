#pragma once

#include <type_traits>
#include <functional>
#include "boost/type_traits.hpp"

namespace planeta {
	namespace private_ {
		//LuaIntfにプロパティセッターとして登録するために、戻り値をvoid化する。
		/*戻り値がvoidの場合はそのまま*/
		template<class C, typename Ret, typename... Param>
		auto GetMemberPropertySetterToLuaIntf(Ret(C::*mf_ptr)(Param...)) -> typename boost::enable_if<std::is_same<void,Ret>,Ret(C::*)(Param...)>::type{
			return mf_ptr;
		}
		//戻り値が非voidの場合は戻り値がvoidのラムダ式でラップする
		template<class C, typename Ret, typename... Params>
		auto GetMemberPropertySetterToLuaIntf(Ret(C::*mf_ptr)(Params...)) -> typename boost::disable_if<std::is_same<void, Ret>, std::function<void(C*, Params...)>>::type {
			return [mf_ptr](C* ptr, Params... params) {(ptr->*mf_ptr)(params...); };
		}
		template<class C, typename Ret, typename... Params>
		auto GetMemberPropertySetterToLuaIntf(Ret(C::*mf_ptr)(Params...)const) -> typename boost::disable_if<std::is_same<void, Ret>, std::function<void(const C*, Params...)>>::type {
			return [mf_ptr](const C* ptr, Params... params) {(ptr->*mf_ptr)(params...); };
		}
	}
}