#pragma once

#include <string>
#include <functional>
#include <typeinfo>
#include <iostream>
#include <type_traits>

#include "boost/call_traits.hpp"
#include "boost/lexical_cast.hpp"

namespace plnt::mp_util {
	struct nil { };

	/*ペアを作成*/
	template <typename First, typename Rest>
	struct cons {
		using first = First;
		using rest = Rest;
	};

	/*リストを作成*/
	template <typename TFirst, typename... TRest>
	struct meta_list {
		using list = cons<TFirst, meta_list<TRest...>>;
	};

	template <>
	struct meta_list<void> {
		using list = nil;
	};

	/*リストに処理を行う*/
	template <typename List, template<typename>typename Proc>
	struct apply_to_meta_list {
		using list = cons<typename Proc<typename List::first>::type, typename apply_to_meta_list<
			                  typename List::rest, Proc>::list>;
	};

	template <template<typename>typename Proc>
	struct apply_to_meta_list<nil, Proc> {
		using list = nil;
	};

	/*タイプリストを最適な引数リストに変換*/
	template <typename List>
	struct convert_to_param_list {
	private:
		template <typename T>
		struct boost_call_traits_param_type_wrapper {
			using type = typename boost::call_traits<T>::param_type;
		};

	public:
		using param_list = typename apply_to_meta_list<List, boost_call_traits_param_type_wrapper>::list;
	};

	/*関数の型を表す*/
	template <typename... Params>
	struct function_type {
		template <typename Ret>
		using func_type = std::function<Ret(Params...)>;
		template <typename Ret>
		using func_pointer_type = Ret(*)(Params...);
		template <class C, typename Ret>
		using member_func_pointer_type = Ret(C::*)(Params...);

		template <typename Ret>
		static Ret call(const func_type<Ret> &func, Params... params) { return func(params...); }
	};

	/*引数リストを展開(FuncType<Params...>に展開する)*/
	template <typename ParamList, typename... Params>
	struct expand_parameter_list2 {
		using type = typename expand_parameter_list2<
			typename ParamList::rest, typename ParamList::first, Params...>::type;
	};

	template <typename... Params>
	struct expand_parameter_list2<nil, Params...> {
		using type = function_type<Params...>;
	};

	template <typename ParamList>
	struct expand_parameter_list {
		using type = typename expand_parameter_list2<typename ParamList::rest, typename ParamList::first>::type;
	};

	//! 引数を文字列化
	template <typename FirstParam, typename... Params>
	std::string convert_parameters_to_string(FirstParam first, Params... rest) {
		return std::move(
			std::string(typeid(FirstParam).name()) + " " + boost::lexical_cast<std::string>(first) + "," +
			ConvertParametersToString(rest...));
	};

	template <typename FirstParam>
	std::string convert_parameters_to_string(FirstParam first) {
		return std::move(std::string(typeid(FirstParam).name()) + " " + boost::lexical_cast<std::string>(first));
	};

	//! 引数の型を文字列化
	template <typename FirstParam, typename... Params>
	std::string convert_parameter_types_to_string() {
		return std::move(std::string(typeid(FirstParam).name()) + "," + convert_parameter_types_to_string<Params...>());
	};

	template <typename FirstParam>
	std::string convert_parameter_types_to_string() { return std::move(std::string(typeid(FirstParam).name())); };

	//! 全ての要素First,Rest...が単項述語UnaryPredicateを満たしているか
	template <template<class> class UnaryPredicate, typename... Args>
	struct all_of : std::true_type { };

	template <template<class> class UnaryPredicate, typename First, typename... Rest>
	struct all_of<UnaryPredicate, First, Rest...> : std::conditional_t<
			UnaryPredicate<First>::value, all_of<UnaryPredicate, Rest...>, std::false_type> { };

	//! いずれかの要素First,Rest...が単項述語UnaryPredicateを満たしているか
	template <template<class> class UnaryPredicate, typename... Args>
	struct any_of : std::false_type { };

	template <template<class> class UnaryPredicate, typename First, typename... Rest>
	struct any_of<UnaryPredicate, First, Rest...> : std::conditional_t<
			UnaryPredicate<First>::value, std::true_type, any_of<UnaryPredicate, Rest...>> { };

	//! メンバ関数の引数の数を取得する
	template <class C, typename Ret, typename... Args>
	constexpr int get_function_arg_count(Ret (C::*)(Args...)) { return sizeof...(Args); }

	//! 関数の引数の数を取得する
	template <class C, typename Ret, typename... Args>
	constexpr int get_function_arg_count(Ret (Args...)) { return sizeof...(Args); }

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
	template <typename T, typename U = void>
	struct is_istream_compatible : std::false_type { };

	template <typename T>
	struct is_istream_compatible<T, decltype(std::declval<std::istream &>() >> std::declval<T &>(), std::declval<
			                           void>()
	                           )> : std::true_type { };

	template <typename T>
	constexpr bool is_istream_compatible_v = is_istream_compatible<T>::value;

	//出力ストリームに対応しているかどうかを判断するメタ関数
	template <typename T, typename U = void>
	struct is_ostream_compatible : std::false_type { };

	template <typename T>
	struct is_ostream_compatible<T, decltype(std::declval<std::ostream &>() << std::declval<T &>(), std::declval<
			                           void>()
	                           )> : std::true_type { };

	template <typename T>
	constexpr bool is_ostream_compatible_v = is_ostream_compatible<T>::value;
}
