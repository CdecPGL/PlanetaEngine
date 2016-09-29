#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include "lua.hpp"
#include "LuaIntf.h"
#include "boost/any.hpp"
#include "boost/type_traits.hpp"
#include "boost/function_types/function_type.hpp"
#include "boost/function_types/parameter_types.hpp"

#include "LuaBindHelder.h"
#include "ClassInfo.h"
#include "ReflectionUtility.h"


namespace planeta {
	/*! @brief リフレクションシステムにクラス情報を登録するためのクラス
		
		変数、プロパティ、関数を登録することができる。
		登録時にアクセス属性を設定するが、Public属性の物のみリフレクションシステムから呼び出すことができる。
		スクリプトへの登録が指定されているときは、Public属性のものが登録される。
		コピー対象指定がされているものは、属性にかかわらずコピー対象として登録される。
	*/
	template<class C>
	class ClassRegisterer {
	public:
		ClassRegisterer(private_::ClassInfo& ci)noexcept :class_info_(ci) {}
		/*! @brief クラスをスクリプトにバインドするかどうかを設定する
			@param bind_to_script スクリプトにバインドするか
		*/
		ClassRegisterer& SetBindToScriptFlag(bool flag)noexcept { class_info_.is_bind_to_script = flag; }
		/*! @brief 変数を登録する
		*/
		template<typename V>
		ClassRegisterer& Variable(const std::string& var_id, V C::* mv_ptr)noexcept;
		/*! @brief プロパティを登録する。

			getterは引数を持たない型、setterは引数を持つ型でないといけない。
			@todo Gettterの戻り値がconst、Setterの引数がnonconst参照の時のcopy関数のコンパイルエラーを修正したい。Setterの引数がnonconst左辺値参照の場合のset関数のコンパイルエラーを修正したい。
		*/
		template<typename GRet, typename SRet, typename SArg>
		ClassRegisterer& Property(const std::string& prop_id, GRet(C::*getter)(), SRet(C::*setter)(SArg))noexcept;
		template<typename GRet, typename SRet, typename SArg>
		ClassRegisterer& Property(const std::string& prop_id, GRet(C::*getter)()const, SRet(C::*setter)(SArg))noexcept;
		/*! @brief 読み取り専用プロパティを登録する
		*/
		template<typename Getter>
		ClassRegisterer& ReadOnlyProperty(const std::string& prop_id, const Getter& getter)noexcept;
		/*! @brief 書き込み専用プロパティを登録する
		*/
		template<typename Setter>
		ClassRegisterer& WriteOnlyProperty(const std::string& prop_id, const Setter& setter)noexcept;
		/*! @brief メンバ関数を追加する
		*/
		template<typename Func>
		ClassRegisterer& Function(const std::string& func_id, const Func& func)noexcept;
		/*! ShallowCopy対象を追加(代入演算子が定義されている必要がある)*/
		template<typename V>
		ClassRegisterer& ShallowCopyTarget(V C::* var)noexcept {
			static_assert(std::is_copy_assignable_v<V>, "V must be copy assignable.");
			class_info_.copy_handler_list.push_back([var](const Reflectable& src, Reflectable& dst) { util::ReflectiveCopyHandler(static_cast<C&>(dst).*var, static_cast<const C&>(src).*var); });
			return *this;
		}
		/*! DeepCopy対象を追加(間接参照演算子*が定義されていて、参照演算子が返す型に代入演算子が定義されている必要がある)*/
		template<typename V>
		ClassRegisterer& DeepCopyTarget(V C::* var)noexcept {
			static_assert(boost::has_dereference<V>::value, "V must have a dereference operator(*).");
			static_assert(std::is_copy_assignable_v<decltype(V().operator*())>, "A type of V's dereference return must be copy assignable.");
			class_info_.copy_handler_list.push_back([var](const Reflectable& src, Reflectable& dst) { util::ReflectiveCopyHandler(*(static_cast<C&>(dst).*var), *(static_cast<const C&>(src).*var)); });
			return *this;
		}
		/*! コピーハンドラを追加*/
		ClassRegisterer& CopyHandler(const std::function<void(const C&, C&)>& handler)noexcept {
			class_info_.copy_handler_list.push_back([handler](const Reflectable& src, Reflectable& dst) {handler(static_cast<const C&>(src), static_cast<C&>(dst)); });
			return *this;
		}
	protected:
		private_::ClassInfo& class_info_;
		template<typename PType,typename Getter, typename Setter>
		ClassRegisterer& Property_(const std::string& prop_id, const Getter& getter, const Setter& setter)noexcept;
		using LuaBinderRet = decltype(std::declval<LuaIntf::LuaBinding>().beginClass<C>(""));
		//引数は最初に事項される関数
		std::function<LuaBinderRet(const std::function<LuaBinderRet()>&)> lua_binder_core_;
	};

	template<class C>
	template<typename V>
	ClassRegisterer<C>& ClassRegisterer<C>::Variable(const std::string& var_id, V C::* mv_ptr)noexcept {
		private_::VariableOrPropertyInfo vpinfo{ typeid(V) };
		vpinfo.is_readable = true;
		vpinfo.is_writable = true;
		vpinfo.getter = [mv_ptr](Reflectable& c) {
			return std::move(boost::any(static_cast<C&>(c).*mv_ptr));
		};
		vpinfo.setter = [mv_ptr](Reflectable& c, const boost::any& v) {
			static_cast<C&>(c).*mv_ptr = boost::any_cast<V>(v);
		};
		vpinfo.ptree_loeder = [mv_ptr](Reflectable& c, const boost::property_tree::ptree& pt) {
			V& rdat = static_cast<C&>(c).*mv_ptr;
			util::ReflectivePtreeConverter(rdat, pt);
		};
		class_info_.public_variable_prpperty_info.emplace(var_id, vpinfo);

		if (class_info_.is_bind_to_script) {
			lua_binder_core_ = [lb = lua_binder_core_, var_id, mv_ptr](const std::function<LuaBinderRet()>& f) {return lb(f).addVariableRef(var_id.c_str(), mv_ptr); };
		}
		return *this;
	}

	/*! @brief プロパティを登録する
	*/
	template<class C>
	template<typename GRet, typename SRet, typename SArg>
	ClassRegisterer<C>& ClassRegisterer<C>::Property(const std::string& prop_id, GRet(C::*getter)(), SRet(C::*setter)(SArg))noexcept {
		using PTypeS = std::remove_cv_t<std::remove_reference_t<SArg>>;
		using PTypeG = std::remove_cv_t<std::remove_reference_t<GRet>>;
		static_assert(std::is_same<PTypeS, PTypeG>::value, "The type of getter or setter is incorrect.");
		return Property_<PTypeS>(prop_id, getter, setter);
	}
	template<class C>
	template<typename GRet, typename SRet, typename SArg>
	ClassRegisterer<C>& ClassRegisterer<C>::Property(const std::string& prop_id, GRet(C::*getter)()const, SRet(C::*setter)(SArg))noexcept {
		using PTypeS = std::remove_cv_t<std::remove_reference_t<SArg>>;
		using PTypeG = std::remove_cv_t<std::remove_reference_t<GRet>>;
		static_assert(std::is_same<PTypeS, PTypeG>::value, "The type of getter or setter is incorrect.");
		return Property_<PTypeS>(prop_id, getter, setter);
	}

	template<class C>
	template<typename PType, typename Getter, typename Setter>
	ClassRegisterer<C>& ClassRegisterer<C>::Property_(const std::string& prop_id, const Getter& getter, const Setter& setter)noexcept {
		private_::VariableOrPropertyInfo pinfo{ typeid(PType) };
		pinfo.is_readable = true;
		pinfo.is_writable = true;
		pinfo.getter = [getter](Reflectable& c) {
			return boost::any((static_cast<C&>(c).*getter)());
		};
		pinfo.setter = [setter](Reflectable& c, const boost::any& v) {
			(static_cast<C&>(c).*setter)(boost::any_cast<PType>(v));
		};
		pinfo.ptree_loeder = [setter](Reflectable& c, const boost::property_tree::ptree& pt) {
			PType dat{};
			util::ReflectivePtreeConverter(dat, pt);
			(static_cast<C&>(c).*setter)(dat);
		};
		class_info_.public_variable_prpperty_info.emplace(prop_id, pinfo);

		if (class_info_.is_bind_to_script) {
			//LuaIntfは戻り値がvoidでないsetterに対応していないので、必要なら戻り値をvoid化する。
			auto bsetter = private_::GetMemberPropertySetterToLuaIntf(setter);
			lua_binder_core_ = [lb = lua_binder_core_, prop_id, getter, setter = bsetter](const std::function<LuaBinderRet()>& f) {return lb(f).addProperty(prop_id.c_str(), getter, setter); };
		}
		return *this;
	}
	/*! @brief 読み取り専用プロパティを登録する
	*/
	template<class C>
	template<typename Getter>
	ClassRegisterer<C>& ClassRegisterer<C>::ReadOnlyProperty(const std::string& prop_id, const Getter& getter)noexcept {
		private_::VariableOrPropertyInfo pinfo{ typeid(decltype((std::declval<C>().*getter)())) };
		pinfo.is_readable = true;
		pinfo.is_writable = false;
		pinfo.getter = [getter](Reflectable& c) {
			return boost::any((static_cast<C&>(c).*getter)());
		};
		class_info_.public_variable_prpperty_info.emplace(prop_id, pinfo);
		if (class_info_.is_bind_to_script) {
			lua_binder_core_ = [lb = lua_binder_core_, prop_id, getter](const std::function<LuaBinderRet()>& f) {return lb(f).addPropertyReadOnly(prop_id.c_str(), getter); };
		}
		return *this;
	}
	/*! @brief 書き込み専用プロパティを登録する
	*/
	template<class C>
	template<typename Setter>
	ClassRegisterer<C>& ClassRegisterer<C>::WriteOnlyProperty(const std::string& prop_id, const Setter& setter)noexcept {
		using PType = std::remove_cv_t<std::remove_reference_t<boost::mpl::at_c<boost::function_types::parameter_types<Setter>, 1>::type>>;
		private_::VariableOrPropertyInfo pinfo{ typeid(PType) };
		pinfo.is_readable = false;
		pinfo.is_writable = true;
		pinfo.setter = [setter](Reflectable& c, const boost::any& v) {
			(static_cast<C&>(c).*setter)(boost::any_cast<PType>(v));
		};
		pinfo.ptree_loeder = [setter](Reflectable& c, const boost::property_tree::ptree& pt) {
			PType dat{};
			util::ReflectivePtreeConverter(dat, pt);
			(static_cast<C&>(c).*setter)(dat);
		};
		class_info_.public_variable_prpperty_info.emplace(prop_id, pinfo);
		if (class_info_.is_bind_to_script) {
			//auto getter = []()->PType {return{}; }; //空のゲッターを指定する
			//lua_binder_core_ = [lb = lua_binder_core_, prop_id, getter, setter](const std::function<LuaBinderRet()>& f) {return lb(f).addProperty(prop_id.c_str(), getter, setter); };
		}
		return *this;
	}
	/*! @brief メンバ関数を追加する
	*/
	template<class C>
	template<typename Func>
	ClassRegisterer<C>& ClassRegisterer<C>::Function(const std::string& func_id, const Func& func)noexcept {
		if (class_info_.is_bind_to_script) {
			lua_binder_core_ = [lb = lua_binder_core_, func_id, func](const std::function<LuaBinderRet()>& f) {return lb(f).addFunction(func_id.c_str(), func); };
		}
		return *this;
	}
}

/*! @brief 標準的なプロパティを登録するユーティリティマクロ

	型の変換なしにClassRegisterer::Propertyに登録して、コンパイルエラーにならないようなゲッター、セッターの場合に使用可能。
	registerer.PE_REFLECTABLE_CLASS_PROPERTY(prop);
	のように使用する。
*/
#define PE_REFLECTABLE_CLASS_PROPERTY(ctype, id)\
Property(#id, &ctype::id, &ctype::id)
/*! @brief 書き込み専用プロパティを登録するユーティリティマクロ

型の変換なしにClassRegisterer::Propertyに登録して、コンパイルエラーにならないようなゲッター、セッターの場合に使用可能。
registerer.PE_REFLECTABLE_CLASS_PROPERTY(prop);
のように使用する。
*/
#define PE_REFLECTABLE_CLASS_READONLY_PROPERTY(ctype, id)\
ReadOnlyProperty(#id, &ctype::id)
/*! @brief 書き込み専用プロパティを登録するユーティリティマクロ

型の変換なしにClassRegisterer::Propertyに登録して、コンパイルエラーにならないようなゲッター、セッターの場合に使用可能。
registerer.PE_REFLECTABLE_CLASS_PROPERTY(prop);
のように使用する。
*/
#define PE_REFLECTABLE_CLASS_WRITEONLY_PROPERTY(ctype, id)\
WriteOnlyProperty(#id, &ctype::id)