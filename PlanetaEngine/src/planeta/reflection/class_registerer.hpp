#pragma once

#define DISABLE_SCRIPT_REGISTRATION

#include <string>
#include <unordered_map>
#include <functional>

/*
#include "planeta/core/lua.hpp"
//Luaintfの警告を抑制
#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4100)
#include "planeta/core/LuaIntf.hpp"
#pragma warning(pop)
*/

#include "boost/any.hpp"
#include "boost/type_traits.hpp"
#include "boost/function_types/parameter_types.hpp"

#include "lua_bind_helper.hpp"
#include "class_info.hpp"
#include "reflection_utility.hpp"


namespace plnt::reflection {
	/*! @brief リフレクションシステムにクラス情報を登録するためのクラス
		
		変数、プロパティ、関数を登録することができる。
		登録時にアクセス属性を設定するが、Public属性の物のみリフレクションシステムから呼び出すことができる。
		スクリプトへの登録が指定されているときは、Public属性のものが登録される。
		コピー対象指定がされているものは、属性にかかわらずコピー対象として登録される。
	*/
	template <class C>
	class class_registerer {
	public:
		explicit class_registerer(private_::class_info &ci) noexcept : class_info_(ci) { }

		/*! @brief クラスをスクリプトにバインドするかどうかを設定する
			@param bind_to_script スクリプトにバインドするか
		*/
		class_registerer &set_bind_to_script_flag(const bool bind_to_script) noexcept {
			class_info_.is_bind_to_script = bind_to_script;
			return *this;
		}

		/*! @brief 変数を登録する
		*/
		template <typename V>
		class_registerer &variable(const std::string &var_id, V C::*mv_ptr) noexcept;
		/*! @brief プロパティを登録する。

			getterは引数を持たない型、setterは引数を持つ型でないといけない。
			@todo Gettterの戻り値がconst、Setterの引数がnonconst参照の時のcopy関数のコンパイルエラーを修正したい。Setterの引数がnonconst左辺値参照の場合のset関数のコンパイルエラーを修正したい。
		*/
		template <typename GRet, typename SRet, typename SArg>
		class_registerer &read_write_property(const std::string &prop_id, GRet (C::*getter)(),
		                                      SRet (C::*setter)(SArg)) noexcept;
		template <typename GRet, typename SRet, typename SArg>
		class_registerer &read_write_property(const std::string &prop_id, GRet (C::*getter)() const,
		                                      SRet (C::*setter)(SArg)) noexcept;
		/*! @brief 読み取り専用プロパティを登録する
		*/
		template <typename Getter>
		class_registerer &read_only_property(const std::string &prop_id, const Getter &getter) noexcept;
		/*! @brief 書き込み専用プロパティを登録する
		*/
		template <typename Setter>
		class_registerer &write_only_property(const std::string &prop_id, const Setter &setter) noexcept;
		/*! @brief メンバ関数を追加する
		*/
		template <typename Func>
		class_registerer &function(const std::string &func_id, const Func &func) noexcept;
		/*! ShallowCopy対象を追加(代入演算子が定義されている必要がある)*/
		template <typename V>
		class_registerer &shallow_copy_target(V C::*var) noexcept {
			static_assert(std::is_copy_assignable_v<V>, "V must be copy assignable.");
			class_info_.copy_handler_list.push_back([var](const reflectable &src, reflectable &dst) {
				reflective_copy_handler(static_cast<C &>(dst).*var, static_cast<const C &>(src).*var);
			});
			return *this;
		}

		/*! DeepCopy対象を追加(間接参照演算子*が定義されていて、参照演算子が返す型に代入演算子が定義されている必要がある)*/
		template <typename V>
		class_registerer &deep_copy_target(V C::*var) noexcept {
			static_assert(boost::has_dereference<V>::value, "V must have a dereference operator(*).");
			static_assert(std::is_copy_assignable_v<decltype(V().operator*())>,
			              "A type of V's dereference return must be copy assignable.");
			class_info_.copy_handler_list.push_back([var](const reflectable &src, reflectable &dst) {
				reflective_copy_handler(*(static_cast<C &>(dst).*var), *(static_cast<const C &>(src).*var));
			});
			return *this;
		}

		/*! コピーハンドラを追加*/
		class_registerer &copy_handler(const std::function<void(const C &, C &)> &handler) noexcept {
			class_info_.copy_handler_list.push_back([handler](const reflectable &src, reflectable &dst) {
				handler(static_cast<const C &>(src), static_cast<C &>(dst));
			});
			return *this;
		}

	protected:
		private_::class_info &class_info_;
		template <typename PType, typename Getter, typename Setter>
		class_registerer &property_(const std::string &prop_id, const Getter &getter, const Setter &setter) noexcept;
		#ifndef DISABLE_SCRIPT_REGISTRATION
		using LuaBinderRet = decltype(std::declval<LuaIntf::LuaBinding>().beginClass<C>(""));
		//引数は最初に事項される関数
		std::function<LuaBinderRet(const std::function<LuaBinderRet()>&)> lua_binder_core_;
		#endif
	};

	template <class C>
	template <typename V>
	class_registerer<C> &class_registerer<C>::variable(const std::string &var_id, V C::*mv_ptr) noexcept {
		private_::variable_or_property_info vp_info{typeid(V)};
		vp_info.is_readable = true;
		vp_info.is_writable = true;
		vp_info.getter = [mv_ptr](reflectable &c) { return std::move(boost::any(static_cast<C &>(c).*mv_ptr)); };
		vp_info.setter = [mv_ptr](reflectable &c, const boost::any &v) {
			static_cast<C &>(c).*mv_ptr = boost::any_cast<V>(v);
		};
		vp_info.ptree_loader = [mv_ptr](reflectable &c, const boost::property_tree::ptree &pt) {
			V &data = static_cast<C &>(c).*mv_ptr;
			reflective_ptree_converter(data, pt);
		};
		class_info_.public_variable_property_info.emplace(var_id, vp_info);

		#ifndef DISABLE_SCRIPT_REGISTRATION
		if (class_info_.is_bind_to_script) {
			lua_binder_core_ = [lb = lua_binder_core_, var_id, mv_ptr](const std::function<LuaBinderRet()>& f) {return lb(f).addVariableRef(var_id.c_str(), mv_ptr); };
		}
		#endif
		return *this;
	}

	/*! @brief プロパティを登録する
	*/
	template <class C>
	template <typename GRet, typename SRet, typename SArg>
	class_registerer<C> &class_registerer<C>::read_write_property(const std::string &prop_id, GRet (C::*getter)(),
	                                                              SRet (C::*setter)(SArg)) noexcept {
		using p_type_s = std::remove_cv_t<std::remove_reference_t<SArg>>;
		using p_type_g = std::remove_cv_t<std::remove_reference_t<GRet>>;
		static_assert(std::is_same_v<p_type_s, p_type_g>, "The type of getter or setter is incorrect.");
		return property_<p_type_s>(prop_id, getter, setter);
	}

	template <class C>
	template <typename GRet, typename SRet, typename SArg>
	class_registerer<C> &class_registerer<C>::read_write_property(const std::string &prop_id, GRet (C::*getter)() const,
	                                                              SRet (C::*setter)(SArg)) noexcept {
		using p_type_s = std::remove_cv_t<std::remove_reference_t<SArg>>;
		using p_type_g = std::remove_cv_t<std::remove_reference_t<GRet>>;
		static_assert(std::is_same_v<p_type_s, p_type_g>, "The type of getter or setter is incorrect.");
		return property_<p_type_s>(prop_id, getter, setter);
	}

	template <class C>
	template <typename PType, typename Getter, typename Setter>
	class_registerer<C> &class_registerer<C>::property_(const std::string &prop_id, const Getter &getter,
	                                                    const Setter &setter) noexcept {
		private_::variable_or_property_info vp_info{typeid(PType)};
		vp_info.is_readable = true;
		vp_info.is_writable = true;
		vp_info.getter = [getter](reflectable &c) { return boost::any((static_cast<C &>(c).*getter)()); };
		vp_info.setter = [setter](reflectable &c, const boost::any &v) {
			(static_cast<C &>(c).*setter)(boost::any_cast<PType>(v));
		};
		vp_info.ptree_loader = [setter](reflectable &c, const boost::property_tree::ptree &pt) {
			PType dat{};
			reflective_ptree_converter(dat, pt);
			(static_cast<C &>(c).*setter)(dat);
		};
		class_info_.public_variable_property_info.emplace(prop_id, vp_info);

		#ifndef DISABLE_SCRIPT_REGISTRATION
		if (class_info_.is_bind_to_script) {
			//LuaIntfは戻り値がvoidでないsetterに対応していないので、必要なら戻り値をvoid化する。
			auto bsetter = private_::GetMemberPropertySetterToLuaIntf(setter);
			lua_binder_core_ = [lb = lua_binder_core_, prop_id, getter, setter = bsetter](const std::function<LuaBinderRet()>& f) {return lb(f).addProperty(prop_id.c_str(), getter, setter); };
		}
		#endif
		return *this;
	}

	/*! @brief 読み取り専用プロパティを登録する
	*/
	template <class C>
	template <typename Getter>
	class_registerer<C> &class_registerer<
		C>::read_only_property(const std::string &prop_id, const Getter &getter) noexcept {
		private_::variable_or_property_info vp_info{typeid(decltype((std::declval<C>().*getter)()))};
		vp_info.is_readable = true;
		vp_info.is_writable = false;
		vp_info.getter = [getter](reflectable &c) { return boost::any((static_cast<C &>(c).*getter)()); };
		class_info_.public_variable_property_info.emplace(prop_id, vp_info);
		#ifndef DISABLE_SCRIPT_REGISTRATION
		if (class_info_.is_bind_to_script) {
			lua_binder_core_ = [lb = lua_binder_core_, prop_id, getter](const std::function<LuaBinderRet()>& f) {return lb(f).addPropertyReadOnly(prop_id.c_str(), getter); };
		}
		#endif
		return *this;
	}

	/*! @brief 書き込み専用プロパティを登録する
	*/
	template <class C>
	template <typename Setter>
	class_registerer<C> &class_registerer<
		C>::write_only_property(const std::string &prop_id, const Setter &setter) noexcept {
		using p_type = std::remove_cv_t<std::remove_reference_t<typename boost::mpl::at_c<
			boost::function_types::parameter_types<Setter>, 1>::type>>;
		private_::variable_or_property_info vp_info{typeid(p_type)};
		vp_info.is_readable = false;
		vp_info.is_writable = true;
		vp_info.setter = [setter](reflectable &c, const boost::any &v) {
			(static_cast<C &>(c).*setter)(boost::any_cast<p_type>(v));
		};
		vp_info.ptree_loader = [setter](reflectable &c, const boost::property_tree::ptree &pt) {
			p_type dat{};
			reflective_ptree_converter(dat, pt);
			(static_cast<C &>(c).*setter)(dat);
		};
		class_info_.public_variable_property_info.emplace(prop_id, vp_info);
		#ifndef DISABLE_SCRIPT_REGISTRATION
		if (class_info_.is_bind_to_script) {
			//auto getter = []()->PType {return{}; }; //空のゲッターを指定する
			//lua_binder_core_ = [lb = lua_binder_core_, prop_id, getter, setter](const std::function<LuaBinderRet()>& f) {return lb(f).addProperty(prop_id.c_str(), getter, setter); };
		}
		#endif
		return *this;
	}

	/*! @brief メンバ関数を追加する
	*/
	template <class C>
	template <typename Func>
	class_registerer<C> &class_registerer<C>::function(const std::string &func_id, const Func &func) noexcept {
		#ifndef DISABLE_SCRIPT_REGISTRATION
		if (class_info_.is_bind_to_script) {
			lua_binder_core_ = [lb = lua_binder_core_, func_id, func](const std::function<LuaBinderRet()>& f) {return lb(f).addFunction(func_id.c_str(), func); };
		}
		#endif
		return *this;
	}
}

/*! @brief 標準的なプロパティを登録するユーティリティマクロ

型の変換なしにClassRegisterer::read_write_propertyに登録して、コンパイルエラーにならないようなゲッター、セッターの場合に使用可能。
registerer.PE_REFLECTABLE_CLASS_PROPERTY(prop);
のように使用する。
*/
#define PE_REFLECTABLE_CLASS_PROPERTY(c_type, id)                             \
read_write_property (#id, &c_type::id, &c_type::id)
/*! @brief 書き込み専用プロパティを登録するユーティリティマクロ

型の変換なしにclass_registerer::read_only_propertyに登録して、コンパイルエラーにならないようなゲッター、セッターの場合に使用可能。
registerer.PE_REFLECTABLE_CLASS_PROPERTY(prop);
のように使用する。
*/
#define PE_REFLECTABLE_CLASS_READONLY_PROPERTY(c_type, id)                    \
read_only_property(#id, &c_type::id)
/*! @brief 書き込み専用プロパティを登録するユーティリティマクロ

型の変換なしにclass_registerer::write_only_propertyに登録して、コンパイルエラーにならないようなゲッター、セッターの場合に使用可能。
registerer.PE_REFLECTABLE_CLASS_PROPERTY(prop);
のように使用する。
*/
#define PE_REFLECTABLE_CLASS_WRITEONLY_PROPERTY(c_type, id)                   \
write_only_property(#id, &c_type::id)
/*! @brief 変数を登録するユーティリティマクロ

型の変換なしにclass_registerer::variableに登録して、コンパイルエラーにならないような変数の場合に使用可能。
registerer.PE_REFLECTABLE_CLASS_PROPERTY(prop);
のように使用する。
*/
#define PE_REFLECTABLE_CLASS_VARIABLE(c_type, id)\
variable(#id, &c_type::id)
