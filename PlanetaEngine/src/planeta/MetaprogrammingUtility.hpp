#pragma once

#include <string>
#include <functional>
#include <typeinfo>
#include <iostream>
#include <type_traits>
#include "boost/any.hpp"
#include "boost/call_traits.hpp"

namespace plnt {
	namespace mp_util{
		struct nil {};

		/*ペアを作成*/
		template<typename First, typename Rest>
		struct Cons {
			using first = First;
			using rest = Rest;
		};
		/*リストを作成*/
		template<typename TFirst, typename ... TRest>
		struct List {
			using list = Cons<TFirst, List<TRest...>>;
		};
		template<>
		struct List<void> {
			using list = nil;
		};
		/*リストに処理を行う*/
		template<typename List,template<typename>typename Proc>
		struct ApplyToList {
			using list = Cons<typename Proc<typename List::first>::type,typename ApplyToList<typename List::rest,Proc>::list>;
		};
		template<template<typename>typename Proc>
		struct ApplyToList<nil,Proc> {
			using list = nil;
		};

		/*タイプリストを最適な引数リストに変換*/
		template<typename List>
		struct ConvertParameterList {
		private:
			template<typename T>
			struct BoostCallTraitsParamTypeWrapper { using type = typename boost::call_traits<T>::param_type; };
		public:
			using param_list = typename ApplyToList<List, BoostCallTraitsParamTypeWrapper>::list;
		};
		/*関数の型を表す*/
		template<typename ... Params>
		struct FunctionType {
			template<typename Ret>
			using func_type = std::function<Ret(Params...)>;
			template<typename Ret>
			using func_pointer_type = Ret(*)(Params...);
			template<class C, typename Ret>
			using mfunc_pointer_type = Ret(C::*)(Params...);
			template<typename Ret>
			static Ret Call(const func_type<Ret>& func, Params... params) {
				return func(params...);
			}
		};
		/*引数リストを展開(FuncType<Params...>に展開する)*/
		template<typename ParamList, typename ... Params>
		struct ExpandParameterList2 {
			using type = typename ExpandParameterList2<typename ParamList::rest, typename ParamList::first, Params...>::type;
		};
		template<typename ... Params>
		struct ExpandParameterList2<nil, Params...> {
			using type = FunctionType<Params...>;
		};
		template<typename ParamList>
		struct ExpandParameterList {
			using type = typename ExpandParameterList2<typename ParamList::rest, typename ParamList::first>::type;
		};

		//! 引数を文字列化
		template<typename FirstParam, typename ... Params>
		std::string ConvertParametersToString(FirstParam fparam, Params... params) {
			return std::move(std::string(typeid(FirstParam).name()) + " " + boost::lexical_cast<std::string>(fparam) + "," + ConvertParametersToString(params...));
		};
		template<typename FirstParam>
		std::string ConvertParametersToString(FirstParam fparam) {
			return std::move(std::string(typeid(FirstParam).name()) + " " + boost::lexical_cast<std::string>(fparam));
		};

		//! 引数の型を文字列化
		template<typename FirstParam, typename ... Params>
		std::string ConvertParameterTypesToString() {
			return std::move(std::string(std::typeid(FirstParam).name()) + "," + ConvertParameterTypesToString<Params...>());
		};
		template<typename FirstParam>
		std::string ConvertParameterTypesToString() {
			return std::move(std::string(std::typeid(FirstParam).name()));
		};

		//! 全ての要素First,Rest...が単項述語UnaryPredicateを満たしているか
		template<template<class> class UnaryPredicate, typename... Args>
		struct AllOf : public std::true_type {};
		template<template<class> class UnaryPredicate, typename First, typename... Rest>
		struct AllOf<UnaryPredicate, First, Rest...> : public std::conditional_t<UnaryPredicate<First>::value, AllOf<UnaryPredicate, Rest...>, std::false_type> {};
		/*template<template<class>class UnaryPredicate>
		struct AllOf<UnaryPredicate, void> : public std::true_type{};*/

		//! いずれかの要素First,Rest...が単項述語UnaryPredicateを満たしているか
		template<template<class> class UnaryPredicate, typename... Args>
		struct AnyOf : public std::false_type {};
		template<template<class> class UnaryPredicate, typename First, typename... Rest>
		struct AnyOf<UnaryPredicate, First, Rest...> : public std::conditional_t<UnaryPredicate<First>::value,std::true_type, AnyOf<UnaryPredicate, Rest...>> {};

		//! メンバ関数の引数の数を取得する
		template<class C, typename Ret, typename... Args>
		constexpr int GetFunctionArgCount(Ret(C::*)(Args...)) {
			return sizeof...(Args);
		}
		//! 関数の引数の数を取得する
		template<class C, typename Ret, typename... Args>
		constexpr int GetFunctionArgCount(Ret(Args...)) {
			return sizeof...(Args);
		}

		//引数リストからメンバ関数を呼び出す関数と、引数リストから関数を呼び出す関数を取得する関数。コンパイルの確認すらしていないので、エラーになる可能性が高い。
		/*template<class C, typename Ret, typename... Args, int N, int IDX, typename... FArgs>
		Ret CallMemFuncWithParamArray(std::enable_if_t<!N, C*> cptr, Ret(C::*)(Args...) mpfunc, FArgs... fargs) {
			return (cptr->*mpfunc)(fargs...);
		}

		template<class C, typename Ret, typename FirstArg, typename... RestArgs, int N, int IDX, typename... FArgs>
		Ret CallMemFuncWithParamArray(std::enable_if_t<N, C*> cptr, Ret(C::*)(Args...) mpfunc, const std::vector<boost::any>& arg_list, FArgs... fargs) {
			return CallMemFuncWithParamArray<C, Ret, RestArgs..., N, IDX + 1, FArgs..., FirstArg>(cptr, mpfunc, arg_list, fargs..., boost::any_cast<FirstArg>(arg_list.at(IDX)));
		}

		template<class C, typename Ret, typename... Args>
		Ret CallMemFuncWithParamArray(C* cptr, Ret(C::*mfunc)(Args...), const std::vector<boost::any>& arg_list) {
			return CallMemFuncWithParamArray<C, Ret, Args..., sizeof...(Args), 0>(cptr, mfunc, arg_list);
		}

		template<class C, typename Ret, typename... Args>
		std::function<boost::any(void*, const std::vector<boost::any>&)> GetMemFuncCaller(Ret(C::*mpfunc)(Args...)) {
			return [](void* cptr, const std::vector<boost::any>& arg_list) {CallMemFuncWithParamArray(reinterpret_cast<C*>(cptr), mpfunc, arg_list); };*/

		//入力ストリームに対応しているかどうかを判断するメタ関数
		template<typename T, typename U = void>
		struct IsIStreamCompatible : public std::false_type {};
		template<typename T>
		struct IsIStreamCompatible<T, decltype(std::declval<std::istream&>() >> std::declval<T&>(), std::declval<void>())> : public std::true_type {};
		template<typename T>
		constexpr bool IsIStreamCompatible_v = IsIStreamCompatible<T>::value;

		//出力ストリームに対応しているかどうかを判断するメタ関数
		template<typename T, typename U = void>
		struct IsOStreamCompatible : public std::false_type {};
		template<typename T>
		struct IsOStreamCompatible<T, decltype(std::declval<std::ostream&>() << std::declval<T&>(), std::declval<void>())> : public std::true_type {};
		template<typename T>
		constexpr bool IsOStreamCompatible_v = IsOStreamCompatible<T>::value;
	}
}
