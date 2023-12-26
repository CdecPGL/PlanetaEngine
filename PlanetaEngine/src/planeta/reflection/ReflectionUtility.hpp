#pragma once

#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>

#include <map>
#include <unordered_map>

#include <type_traits>
#include <iostream>

#include "boost/property_tree/ptree.hpp"
#include "boost/core/enable_if.hpp"
#include "boost/tti/has_static_member_function.hpp"

#include "planeta/core/MetaprogrammingUtility.hpp"
#include "planeta/core/StringUtility.hpp"
#include "planeta/reflection/ReflectionExceptions.hpp"

namespace plnt::reflection {
	class Reflectable;
	class ReflectionAccessible;
	template <class C>
	class ClassRegisterer;

	//////////////////////////////////////////////////////////////////////////
	//特定の条件を満たしているか確認するメタ関数
	//////////////////////////////////////////////////////////////////////////
	namespace private_ {
		BOOST_TTI_HAS_STATIC_MEMBER_FUNCTION(ReflectionDataRegisterer)

		//C::ReflectionDataRegisterer(ClassRegisterer<C>&)を持っているか
		template <class C>
		struct HasReflectionDataRegisterer : has_static_member_function_ReflectionDataRegisterer<
				C, void, boost::mpl::vector<ClassRegisterer<C> &>> { };

		template <class C>
		bool HasReflectionDataRegisterer_v = HasReflectionDataRegisterer<C>::value;

		////C::Superを持っているか
		template <class C, typename T = void>
		struct HasSuperAlias : std::false_type { };

		template <class C>
		struct HasSuperAlias<C, decltype(std::declval<typename C::Super>(), std::declval<void>())> : std::true_type { };

		template <class C>
		bool HasSuperAlias_v = HasSuperAlias<C>::value;
	}

	//////////////////////////////////////////////////////////////////////////
	//Ptreeからの変換関数
	//////////////////////////////////////////////////////////////////////////

	template <typename T>
	void ReflectivePtreeConverter(T &dst, const boost::property_tree::ptree &src);

	namespace private_ {
		template <typename T, typename... Rest>
		void ReflectivePtreeConverterError() {
			throw reflection_error(
				util::ConvertAndConnectToString("Ptreeからの変換に対応していない型\"", typeid(T).name(), "\"が指定されました。"));
		}

		void ReflectivePtreeConverterFromReflectionSystem(Reflectable &dst, const boost::property_tree::ptree &src);

		/*! @brief ptreeから直接変換可能な型へのPtree変換関数(Layer2)
		@note BoostLibrary1.64.0では、get_value内でistreamによる型の変換を行っている。これを利用して、get_valueに対応していない型に対しては、std::cinでもコンパイルエラーになることを利用してSFINEを用いオーバーロード対象外にする。get_valueの内部実装に依存しているため、それに変更があった場合は修正する必要がある。
		*/
		//Ptreeから直接変換不可能。ほかに手段がないのでここに到達した時点でエラー
		template <typename T, typename U = void>
		struct ReflectivePtreeConverter_Layer2 {
			void operator()(T &dst, const boost::property_tree::ptree &src) {
				private_::ReflectivePtreeConverterError<T>();
			}
		};

		//Ptreeから直接変換可能
		template <typename T>
		struct ReflectivePtreeConverter_Layer2<T, std::enable_if_t<mp_util::IsIStreamCompatible_v<T>>> {
			void operator()(T &dst, const boost::property_tree::ptree &src) {
				try { dst = src.get_value<T>(); } catch (boost::property_tree::ptree_bad_data &e) {
					throw reflection_error(
						util::ConvertAndConnectToString("Ptreeから型\"", typeid(T).name(), "\"への変換に失敗しました。(", e.what(),
						                                ")"));
				}
			}
		};

		//Reflectableの継承による分岐(Layer1)
		//非継承
		template <typename T, typename U=void>
		struct ReflectivePtreeConverter_Layer1 {
			void operator()(T &dst, const boost::property_tree::ptree &src) {
				ReflectivePtreeConverter_Layer2<T>()(dst, src);
			}
		};

		//継承
		template <typename T>
		struct ReflectivePtreeConverter_Layer1<T, std::enable_if_t<std::is_base_of_v<Reflectable, T>>> {
			void operator()(T &dst, const boost::property_tree::ptree &src) {
				try { private_::ReflectivePtreeConverterFromReflectionSystem(dst, src); } catch (reflection_error &e) {
					throw reflection_error(
						util::ConvertAndConnectToString("Ptreeから型\"", typeid(T).name(), "\"への変換に失敗しました。(", e.what(),
						                                ")"));
				}
			}
		};
	}

	//特殊型への分岐(Layer0)
	//非特殊型
	template <typename T>
	struct ReflectivePtreeConverterImpl {
		void operator()(T &dst, const boost::property_tree::ptree &src) {
			private_::ReflectivePtreeConverter_Layer1<T>()(dst, src);
		}
	};

	//std::tuple
	namespace private_ {
		template <size_t idx>
		void ReflectivePtreeConverterToStdTuple(std::tuple<> &dst,
		                                        const std::vector<const boost::property_tree::ptree *> &src) { }

		template <size_t idx, typename F, typename... R>
		void ReflectivePtreeConverterToStdTuple(std::tuple<F, R...> &dst,
		                                        const std::vector<const boost::property_tree::ptree *> &src) {
			F f{};
			reflection::ReflectivePtreeConverter(f, *src[idx]);
			std::tuple<R...> rtuple;
			ReflectivePtreeConverterToStdTuple<idx + 1, R...>(rtuple, src);
			dst = std::tuple_cat(std::make_tuple<F>(std::move(f)), rtuple);
		}
	}

	template <typename... Ts>
	struct ReflectivePtreeConverterImpl<std::tuple<Ts...>> {
		void operator()(std::tuple<Ts...> &dst, const boost::property_tree::ptree &src) {
			std::vector<const boost::property_tree::ptree *> ptree_vec;
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::ConvertAndConnectToString("std::tupleのPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}
				ptree_vec.emplace_back(&(pp.second));
			}
			if (sizeof...(Ts) != ptree_vec.size()) {
				throw reflection_error(
					util::ConvertAndConnectToString("要素数が", ptree_vec.size(), "ですが、対象のstd::tupleの要素数は", sizeof...(Ts),
					                                "です。"));
			}
			private_::ReflectivePtreeConverterToStdTuple<0, Ts...>(dst, ptree_vec);
		}
	};

	//std::vector
	template <typename T, typename... Rest>
	struct ReflectivePtreeConverterImpl<std::vector<T, Rest...>> {
		void operator()(std::vector<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::ConvertAndConnectToString("配列型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::ReflectivePtreeConverter(dat, pp.second);
				dst.push_back(std::move(dat));
			}
		}
	};

	//std::list
	template <typename T, typename... Rest>
	struct ReflectivePtreeConverterImpl<std::list<T, Rest...>> {
		void operator()(std::list<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::ConvertAndConnectToString("配列型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::ReflectivePtreeConverter(dat, pp.second);
				dst.push_back(std::move(dat));
			}
		}
	};

	//std::deque
	template <typename T, typename... Rest>
	struct ReflectivePtreeConverterImpl<std::deque<T, Rest...>> {
		void operator()(std::deque<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::ConvertAndConnectToString("配列型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::ReflectivePtreeConverter(dat, pp.second);
				dst.push_back(std::move(dat));
			}
		}
	};

	//std::set
	template <typename T, typename... Rest>
	struct ReflectivePtreeConverterImpl<std::set<T, Rest...>> {
		void operator()(std::set<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::ConvertAndConnectToString("セット型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::ReflectivePtreeConverter(dat, pp.second);
				dst.insert(std::move(dat));
			}
		}
	};

	//std::unordered_set
	template <typename T, typename... Rest>
	struct ReflectivePtreeConverterImpl<std::unordered_set<T, Rest...>> {
		void operator()(std::unordered_set<T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::ConvertAndConnectToString("セット型のPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}

				T dat{};
				reflection::ReflectivePtreeConverter(dat, pp.second);
				dst.insert(std::move(dat));
			}
		}
	};

	//std::map
	template <typename T, typename... Rest>
	struct ReflectivePtreeConverterImpl<std::map<std::string, T, Rest...>> {
		void operator()(std::map<std::string, T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == true) {
					throw reflection_error(util::ConvertAndConnectToString("マップ型のPtreeキーは空であってはいけません。"));
				}
				T dat{};
				reflection::ReflectivePtreeConverter(dat, pp.second);
				dst.emplace(pp.first, std::move(dat));
			}
		}
	};

	//std::unordered_map
	template <typename T, typename... Rest>
	struct ReflectivePtreeConverterImpl<std::unordered_map<std::string, T, Rest...>> {
		void operator()(std::unordered_map<std::string, T, Rest...> &dst, const boost::property_tree::ptree &src) {
			for (auto &&pp : src) {
				if (pp.first.empty() == true) {
					throw reflection_error(util::ConvertAndConnectToString("マップ型のPtreeキーは空であってはいけません。"));
				}
				T dat{};
				reflection::ReflectivePtreeConverter(dat, pp.second);
				dst.emplace(pp.first, std::move(dat));
			}
		}
	};

	template <typename T>
	void ReflectivePtreeConverter(T &dst, const boost::property_tree::ptree &src) {
		ReflectivePtreeConverterImpl<T>()(dst, src);
	}

	//////////////////////////////////////////////////////////////////////////
	//コピーハンドラ
	//////////////////////////////////////////////////////////////////////////
	namespace private_ {
		void ReflectiveCopyFromReflectionSystem(Reflectable &dst, const Reflectable &src);
	}

	/*! コピー代入可能な型のコピーハンドラ*/
	template <typename T>
	auto ReflectiveCopyHandler(
		T &dst, const T &src) -> typename boost::enable_if<std::is_copy_assignable<T>, void>::type { dst = src; }

	/*! コピー代入不可能でReflectionAccessibleを継承している型*/
	template <typename T>
	auto ReflectiveCopyHandler(
		T &dst,
		const T &src) -> typename boost::enable_if_c<
		!std::is_copy_assignable_v<T> && std::is_base_of_v<ReflectionAccessible, T>, void>::type {
		dst.ReflectiveCopyFrom(src);
	}

	/*! コピー代入不可能でReflectionAssignableを継承しておらず、Reflectableを継承している型*/
	template <typename T>
	auto ReflectiveCopyHandler(
		T &dst,
		const T &src) -> typename boost::enable_if_c<
		!std::is_copy_assignable_v<T> && !std::is_base_of_v<ReflectionAccessible, T> && std::is_base_of_v<
			Reflectable, T>, void>::type { private_::ReflectiveCopyFromReflectionSystem(dst, src); }
}
