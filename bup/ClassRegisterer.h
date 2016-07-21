#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include "lua.hpp"
#include "LuaIntf.h"
#include "boost/any.hpp"

namespace planeta {
	//! アクセス属性
	enum class AccessAttiribute { Public, Protected, Private };
	/*! @brief リフレクションシステムにクラス情報を登録するためのクラス

		アクセス属性については、private属性のもの以外がスクリプトに公開される。
	*/
	template<class C>
	class ClassRegisterer {
		struct VariableInfo {
			//コピー関数<void(ソース、ディストリビューション)>
			std::function<void(void*,void*)> copy_func;
		};
		struct PropertyInfo {
			bool is_writable = false;
			bool is_readable = false;
			std::function<void(boost::any)> setter;
			std::function<boost::any(void)> getter;
		};
	public:
		/*! @brief クラスをスクリプトにバインドするかどうかを設定する
			@param bind_to_script スクリプトにバインドするか
		*/
		ClassRegisterer& SetBindToScriptFlag(bool flag) { is_bind_to_script_ = flag; }
		/*! @brief 変数を登録する
		*/
		template<typename V>
		ClassRegisterer& Variable(const std::string& var_id, V T::* mv_ptr, AccessAttiribute access_attribute) {
			VariableInfo var_info;
			var_info.copy_func = [](void* src,void* dst) {
				auto src_ptr = reinterpret_cast<V*>(src);
				auto dst_ptr = reinterpret_cast<V*>(dst);
				*dst_ptr = *src_ptr;
			};
			variable_info_.emplace(var_id, var_info);

			if (is_bind_to_script_ && access_attribute != AccessAttiribute::Private) {
				lua_binder_ = [lb = lua_binder_](std::function<LuaBinderRet()> f) {return lb(f).addVariableRef(var_id.c_str(), mv_ptr); };
			}
			return *this;
		}
		/*! @brief プロパティを登録する
		*/
		template<typename Getter, typename Setter>
		ClassRegisterer& Property(const std::string& prop_id, const Getter& getter, const Setter& setter, AccessAttiribute access_attribute) {
			PropertyInfo pinfo;
			pinfo.is_readable = true;
			pinfo.is_writable = true;
			pinfo.getter = [getter] {

			};


			if (is_bind_to_script_ && access_attribute != AccessAttiribute::Private) {
				lua_binder_ = [lb = lua_binder_](std::function<LuaBinderRet()> f) {return lb(f).addProperty(prop_id.c_str(), getter, setter); };
			}
			return *this;
		}
		/*! @brief 読み取り専用プロパティを登録する
		*/
		template<typename Getter>
		ClassRegisterer& ReadOnlyProperty(const std::string& prop_id, const Getter& getter, AccessAttiribute access_attribute) {
			if (is_bind_to_script_ && access_attribute != AccessAttiribute::Private) {
				lua_binder_ = [lb = lua_binder_](std::function<LuaBinderRet()> f) {return lb(f).addProperty(prop_id.c_str(), getter); };
			}
			return *this;
		}
		/*! @brief 書き込み専用プロパティを登録する
		*/
		template<typename Setter>
		ClassRegisterer& WriteOnlyProperty(const std::string& prop_id, const Setter& setter, AccessAttiribute access_attribute) {
			
			return *this;
		}
		/*! @brief メンバ関数を追加する
		*/
		template<typename Func>
		ClassRegisterer& Function(const std::string& func_id, const Func& func, AccessAttiribute access_attribute) {
			if (is_bind_to_script_ && access_attribute != AccessAttiribute::Private) {
				lua_binder_ = [lb = lua_binder_](std::function<LuaBinderRet()> f) {return lb(f).addFunction(func_id.c_str(), func); };
			}
			return *this;
		}
	protected:
		bool is_bind_to_script_ = true;
		using LuaBinderRet = decltype(LuaIntf::CppBindModule::beginClass<C>());
		std::function<LuaBinderRet(std::function<LuaBinderRet()>)> lua_binder_ = [](std::function<LuaBinderRet()> f) {return f(); };
		std::unordered_map<std::string, VariableInfo> variable_info_;
		std::unordered_map<std::string, PropertyInfo> prpperty_info_;
	};
}