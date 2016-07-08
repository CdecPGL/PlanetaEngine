#pragma once

#include <string>
#include <functional>
#include <typeinfo>
#include "boost/any.hpp"
#include "boost/call_traits.hpp"

namespace planeta {
	namespace mp_utiliey{
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

		/*引数を文字列化*/
		template<typename FirstParam, typename ... Params>
		std::string ConvertParametersToString(FirstParam fparam, Params... params) {
			return std::move(std::string(typeid(FirstParam).name()) + " " + boost::lexical_cast<std::string>(fparam) + "," + ConvertParametersToString(params...));
		};
		template<typename FirstParam>
		std::string ConvertParametersToString(FirstParam fparam) {
			return std::move(std::string(typeid(FirstParam).name()) + " " + boost::lexical_cast<std::string>(fparam));
		};

		/*引数の型を文字列化*/
		template<typename FirstParam, typename ... Params>
		std::string ConvertParameterTypesToString() {
			return std::move(std::string(std::typeid(FirstParam).name()) + "," + ConvertParameterTypesToString<Params...>());
		};
		template<typename FirstParam>
		std::string ConvertParameterTypesToString() {
			return std::move(std::string(std::typeid(FirstParam).name()));
		};

		//全ての要素First,Rest...が単項述語UnaryPredicateを満たしているか
		template<template<class> class UnaryPredicate, typename... Args>
		struct AllOf : public std::true_type {};
		template<template<class> class UnaryPredicate, typename First, typename... Rest>
		struct AllOf<UnaryPredicate, First, Rest...> : public std::conditional_t<UnaryPredicate<First>::value, AllOf<UnaryPredicate, Rest...>, std::false_type> {};
		/*template<template<class>class UnaryPredicate>
		struct AllOf<UnaryPredicate, void> : public std::true_type{};*/

		//いずれかの要素First,Rest...が単項述語UnaryPredicateを満たしているか
		template<template<class> class UnaryPredicate, typename... Args>
		struct AnyOf : public std::false_type {};
		template<template<class> class UnaryPredicate, typename First, typename... Rest>
		struct AnyOf<UnaryPredicate, First, Rest...> : public std::conditional_t<UnaryPredicate<First>::value,std::true_type, AnyOf<UnaryPredicate, Rest...>> {};
	}
}
