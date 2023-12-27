#pragma once

#include <tuple>

namespace plnt::reflection::util {
	/*!タプルに要素を追加*/
	template <typename...>
	struct tuple_add { };

	template <typename... TArgs, typename... NArgs>
	struct tuple_add<std::tuple<TArgs...>, NArgs...> {
		using type = std::tuple<TArgs..., NArgs...>;
	};

	/** @brief スーパークラスをタプルで取得する
			
		superが指定されていないクラスか、Lに達するまでのスーパークラスがタプルで返される。
	*/
	template <typename T, typename L = void, typename U = void>
	struct get_super_class_as_tuple {
		using type = std::tuple<T>;
	};

	template <typename T, typename L>
	struct get_super_class_as_tuple<T, L, decltype(std::declval<typename T::super>(), void())> {
		using type = std::conditional_t<std::is_same_v<T, L>, std::tuple<T>, typename tuple_add<
			                                get_super_class_as_tuple<typename T::super>, T>::type>;
	};
}
