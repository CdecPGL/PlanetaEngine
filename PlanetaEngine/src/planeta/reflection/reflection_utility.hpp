#pragma once

#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <type_traits>

#include "boost/property_tree/ptree.hpp"
#include "boost/core/enable_if.hpp"
#include "boost/tti/has_static_member_function.hpp"

#include "..\core\metaprogramming_utility.hpp"
#include "..\core\string_utility.hpp"
#include "reflection_exceptions.hpp"

namespace plnt::reflection {
	class reflectable;
	class reflection_accessible;
	template <class C>
	class class_registerer;

	//////////////////////////////////////////////////////////////////////////
	//特定の条件を満たしているか確認するメタ関数
	//////////////////////////////////////////////////////////////////////////
	namespace private_ {
		BOOST_TTI_HAS_STATIC_MEMBER_FUNCTION(reflection_data_registerer)

		//C::reflection_data_registerer(ClassRegisterer<C>&)を持っているか
		template <class C>
		struct has_reflection_data_registerer : has_static_member_function_reflection_data_registerer<
				C, void, boost::mpl::vector<class_registerer<C> &>> {};

		template <class C>
		constexpr bool has_reflection_data_registerer_v = has_reflection_data_registerer<C>::value;

		////C::superを持っているか
		template <class C, typename T = void>
		struct has_super_alias : std::false_type {};

		template <class C>
		struct has_super_alias<C, decltype(std::declval<typename C::super>(), std::declval<void>())> : std::true_type {
		};

		template <class C>
		constexpr bool has_super_alias_v = has_super_alias<C>::value;
	}

	//////////////////////////////////////////////////////////////////////////
	//Ptreeからの変換関数
	//////////////////////////////////////////////////////////////////////////

	template <typename T>
	void reflective_ptree_converter(T &dst, const boost::property_tree::ptree &src);

	namespace private_ {
		template <typename T, typename... Rest>
		void reflective_ptree_converter_error() {
			throw reflection_error(
				util::convert_and_connect_to_string("Ptreeからの変換に対応していない型\"", typeid(T).name(), "\"が指定されました。"));
		}

		void reflective_ptree_converter_from_reflection_system(reflectable &dst,
		                                                       const boost::property_tree::ptree &src);

		/*! @brief ptreeから直接変換可能な型へのPtree変換関数(Layer2)
		@note BoostLibrary1.64.0では、get_value内でistreamによる型の変換を行っている。これを利用して、get_valueに対応していない型に対しては、std::cinでもコンパイルエラーになることを利用してSFINEを用いオーバーロード対象外にする。get_valueの内部実装に依存しているため、それに変更があった場合は修正する必要がある。
		*/
		//Ptreeから直接変換不可能。ほかに手段がないのでここに到達した時点でエラー
		template <typename T, typename U = void>
		struct reflective_ptree_converter_layer2 {
			void operator()([[maybe_unused]] T &dst, [[maybe_unused]] const boost::property_tree::ptree &src) {
				private_::reflective_ptree_converter_error<T>();
			}
		};

		//Ptreeから直接変換可能
		template <typename T>
		struct reflective_ptree_converter_layer2<T, std::enable_if_t<mp_util::is_istream_compatible_v<T>>> {
			void operator()(T &dst, const boost::property_tree::ptree &src) {
				try { dst = src.get_value<T>(); } catch (boost::property_tree::ptree_bad_data &e) {
					throw reflection_error(
						util::convert_and_connect_to_string("Ptreeから型\"", typeid(T).name(), "\"への変換に失敗しました。(", e.what(),
						                                    ")"));
				}
			}
		};

		//Reflectableの継承による分岐(Layer1)
		//非継承
		template <typename T, typename U=void>
		struct reflective_ptree_converter_layer1 {
			void operator()(T &dst, const boost::property_tree::ptree &src) {
				reflective_ptree_converter_layer2<T>()(dst, src);
			}
		};

		//継承
		template <typename T>
		struct reflective_ptree_converter_layer1<T, std::enable_if_t<std::is_base_of_v<reflectable, T>>> {
			void operator()(T &dst, const boost::property_tree::ptree &src) {
				try { private_::reflective_ptree_converter_from_reflection_system(dst, src); } catch (reflection_error &
					e) {
					throw reflection_error(
						util::convert_and_connect_to_string("Ptreeから型\"", typeid(T).name(), "\"への変換に失敗しました。(", e.what(),
						                                    ")"));
				}
			}
		};
	}

	//特殊型への分岐(Layer0)
	//非特殊型
	template <typename T>
	struct reflective_ptree_converter_impl {
		void operator()(T &dst, const boost::property_tree::ptree &src) {
			private_::reflective_ptree_converter_layer1<T>()(dst, src);
		}
	};

	//std::tuple
	namespace private_ {
		template <size_t Idx>
		void reflective_ptree_converter_to_std_tuple([[maybe_unused]] std::tuple<> &dst,
		                                             [[maybe_unused]] const std::vector<const
			                                             boost::property_tree::ptree *> &src) {}

		template <size_t Idx, typename F, typename... R>
		void reflective_ptree_converter_to_std_tuple(std::tuple<F, R...> &dst,
		                                             const std::vector<const boost::property_tree::ptree *> &src) {
			F f{};
			reflection::reflective_ptree_converter(f, *src[Idx]);
			std::tuple<R...> rest_tuple;
			reflective_ptree_converter_to_std_tuple<Idx + 1, R...>(rest_tuple, src);
			dst = std::tuple_cat(std::make_tuple<F>(std::move(f)), rest_tuple);
		}
	}

	template <typename... Ts>
	struct reflective_ptree_converter_impl<std::tuple<Ts...>> {
		void operator()(std::tuple<Ts...> &dst, const boost::property_tree::ptree &src) {
			std::vector<const boost::property_tree::ptree *> ptree_vec;
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::convert_and_connect_to_string("std::tupleのPtreeキーは空である必要があります。(読み取られたキー:", pp.first,
						                                    ")"));
				}
				ptree_vec.emplace_back(&(pp.second));
			}
			if (sizeof...(Ts) != ptree_vec.size()) {
				throw reflection_error(
					util::convert_and_connect_to_string("要素数が", ptree_vec.size(), "ですが、対象のstd::tupleの要素数は",
					                                    sizeof...(Ts),
					                                    "です。"));
			}
			private_::reflective_ptree_converter_to_std_tuple<0, Ts...>(dst, ptree_vec);
		}
	};

	//std::vector
	template <typename T, typename... Rest>
	struct reflective_ptree_converter_impl<std::vector<T, Rest...>> {
		void operator()(std::vector<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::convert_and_connect_to_string("配列型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::reflective_ptree_converter(dat, pp.second);
				dst.push_back(std::move(dat));
			}
		}
	};

	//std::list
	template <typename T, typename... Rest>
	struct reflective_ptree_converter_impl<std::list<T, Rest...>> {
		void operator()(std::list<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::convert_and_connect_to_string("配列型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::reflective_ptree_converter(dat, pp.second);
				dst.push_back(std::move(dat));
			}
		}
	};

	//std::deque
	template <typename T, typename... Rest>
	struct reflective_ptree_converter_impl<std::deque<T, Rest...>> {
		void operator()(std::deque<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::convert_and_connect_to_string("配列型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::reflective_ptree_converter(dat, pp.second);
				dst.push_back(std::move(dat));
			}
		}
	};

	//std::set
	template <typename T, typename... Rest>
	struct reflective_ptree_converter_impl<std::set<T, Rest...>> {
		void operator()(std::set<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::convert_and_connect_to_string("セット型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::reflective_ptree_converter(dat, pp.second);
				dst.insert(std::move(dat));
			}
		}
	};

	//std::unordered_set
	template <typename T, typename... Rest>
	struct reflective_ptree_converter_impl<std::unordered_set<T, Rest...>> {
		void operator()(std::unordered_set<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::convert_and_connect_to_string("セット型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::reflective_ptree_converter(dat, pp.second);
				dst.insert(std::move(dat));
			}
		}
	};

	//std::map
	template <typename T, typename... Rest>
	struct reflective_ptree_converter_impl<std::map<std::string, T, Rest...>> {
		void operator()(std::map<std::string, T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == true) {
					throw reflection_error(util::convert_and_connect_to_string("マップ型のPtreeキーは空であってはいけません。"));
				}
				T dat{};
				reflection::reflective_ptree_converter(dat, pp.second);
				dst.emplace(pp.first, std::move(dat));
			}
		}
	};

	//std::unordered_map
	template <typename T, typename... Rest>
	struct reflective_ptree_converter_impl<std::unordered_map<std::string, T, Rest...>> {
		void operator()(std::unordered_map<std::string, T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == true) {
					throw reflection_error(util::convert_and_connect_to_string("マップ型のPtreeキーは空であってはいけません。"));
				}
				T dat{};
				reflection::reflective_ptree_converter(dat, pp.second);
				dst.emplace(pp.first, std::move(dat));
			}
		}
	};

	template <typename T>
	void reflective_ptree_converter(T &dst, const boost::property_tree::ptree &src) {
		reflective_ptree_converter_impl<T>()(dst, src);
	}

	//////////////////////////////////////////////////////////////////////////
	//コピーハンドラ
	//////////////////////////////////////////////////////////////////////////
	namespace private_ {
		void reflective_copy_from_reflection_system(reflectable &dst, const reflectable &src);
	}

	/*! コピー代入可能な型のコピーハンドラ*/
	template <typename T>
	auto reflective_copy_handler(
		T &dst, const T &src) -> typename boost::enable_if<std::is_copy_assignable<T>, void>::type { dst = src; }

	/*! コピー代入不可能でReflectionAccessibleを継承している型*/
	template <typename T>
	auto reflective_copy_handler(
		T &dst,
		const T &src) -> typename boost::enable_if_c<
		!std::is_copy_assignable_v<T> && std::is_base_of_v<reflection_accessible, T>, void>::type {
		dst.ReflectiveCopyFrom(src);
	}

	/*! コピー代入不可能でReflectionAssignableを継承しておらず、Reflectableを継承している型*/
	template <typename T>
	auto reflective_copy_handler(
		T &dst,
		const T &src) -> typename boost::enable_if_c<
		!std::is_copy_assignable_v<T> && !std::is_base_of_v<reflection_accessible, T> && std::is_base_of_v<
			reflectable, T>, void>::type { private_::reflective_copy_from_reflection_system(dst, src); }
}
