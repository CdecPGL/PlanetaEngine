#pragma once

#include "boost/core/enable_if.hpp"

#include <tuple>

namespace plnt {
	namespace util {
		/*!タプルに要素を追加*/
		template<typename...>
		struct TupleAdd {};
		template<typename... TArgs, typename... NArgs>
		struct TupleAdd<std::tuple<TArgs...>, NArgs...> {
			using type = std::tuple<TArgs..., NArgs...>;
		};
		/*! @brief スーパークラスをタプルで取得する
			
			Superが指定されていないクラスか、Lに達するまでのスーパークラスがタプルで返される。
		*/
		template<typename T, typename L = void, typename U = void>
		struct GetSuperClassAsTuple {
			using type = typename std::tuple<T>;
		};
		template<typename T, typename L>
		struct GetSuperClassAsTuple<T, L, decltype(std::declval<typename T::Super>(),void())> {
			using type = std::conditional_t<std::is_same_v<T, L>, std::tuple<T>, typename TupleAdd<GetSuperClassAsTuple<typename T::Super>, T>::type>;
		};
	}
}
