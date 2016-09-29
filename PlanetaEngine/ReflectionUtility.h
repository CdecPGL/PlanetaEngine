﻿#pragma once

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

namespace planeta {
	class Reflectable;
	class ReflectionAccessible;
}

//////////////////////////////////////////////////////////////////////////
//特定の条件を満たしているか確認するメタ関数
//////////////////////////////////////////////////////////////////////////
namespace planeta {
	namespace private_ {
		//C::ReflectionDataRegisterer(ClassRegisterer<C>&)を持っているか
		template<class C, typename T = void>
		struct HasReflectionDataRegisterer : public std::false_type {};
		template<class C>
		struct HasReflectionDataRegisterer < C, decltype(C::ReflectionDataRegisterer(std::declval<ClassRegisterer<C>>()), std::declval<void>()) > : public std::true_type {};
		template<class C>
		bool HasReflectionDataRegisterer_v = HasReflectionDataRegisterer<C>::value;
		//C::Superを持っているか
		template<class C, typename T = void>
		struct HasSuperAlias : public std::false_type {};
		template<class C>
		struct HasSuperAlias < C, decltype(std::declval<typename C::Super>(), std::declval<void>()) > : public std::true_type {};
		template<class C>
		bool HasSuperAlias_v = HasSuperAlias<C>::value;
	}
}

//////////////////////////////////////////////////////////////////////////
//Ptreeからの変換関数
//////////////////////////////////////////////////////////////////////////
#define PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEC(array_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(array_type<T, Rest...>& dst, const boost::property_tree::ptree& src);

#define PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEF(array_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(array_type<T, Rest...>& dst, const boost::property_tree::ptree& src) {\
	for (auto&& pp : src) {\
		if (pp.first.empty() == false) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("配列型のPtreeキーは空である必要があります。(読み取られたキー:",pp.first,")"));\
		}\
		T dat{};\
		ReflectivePtreeConverter(dat, pp.second);\
		dst.push_back(std::move(dat));\
	}\
}

#define PE_PTREE_CONVERT_TO_SET_TYPE_DEC(set_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(set_type<T, Rest...>& dst, const boost::property_tree::ptree& src);

#define PE_PTREE_CONVERT_TO_SET_TYPE_DEF(set_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(set_type<T, Rest...>& dst, const boost::property_tree::ptree& src) {\
	for (auto&& pp : src) {\
		if (pp.first.empty() == false) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("配列型のPtreeキーは空である必要があります。(読み取られたキー:",pp.first,")"));\
		}\
		T dat{};\
		ReflectivePtreeConverter(dat, pp.second);\
		dst.insert(std::move(dat));\
	}\
}

#define PE_PTREE_CONVERT_TO_MAP_TYPE_DEC(map_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(map_type<std::string, T, Rest...>& dst, const boost::property_tree::ptree& src);

#define PE_PTREE_CONVERT_TO_MAP_TYPE_DEF(map_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(map_type<std::string, T, Rest...>& dst, const boost::property_tree::ptree& src) {\
	for (auto&& pp : src) {\
		if (pp.first.empty() == true) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("マップ型のPtreeキーは空であってはいけません。"));\
		}\
		T dat{};\
		ReflectivePtreeConverter(dat, pp.second);\
		dst.emplace(pp.first, std::move(dat));\
	}\
}

namespace planeta {
	namespace private_ {
		template<typename T, typename... Rest>
		void ReflectivePtreeConverterError();
		void ReflectivePtreeConverterFromReflectionSystem(Reflectable& dst, const boost::property_tree::ptree& src);
		template<size_t idx>
		void ReflectivePtreeConverterToStdTuple(std::tuple<>& dst, const std::vector<const boost::property_tree::ptree*>& src);
		template<size_t idx, typename F, typename... R>
		void ReflectivePtreeConverterToStdTuple(std::tuple<F, R...>& dst, const std::vector<const boost::property_tree::ptree*>& src);
	}
	namespace util {
		/*! 変換不可能な型のPtree変換関数*/
		template<typename... Ts>
		void ReflectivePtreeConverter(Ts...);
		/*! @brief ptreeから直接変換可能な型へのPtree変換関数
			@note BoostLibrary1.61.0では、get_value内でistreamによる型の変換を行っている。これを利用して、get_valueに対応していない型に対しては、std::cinでもコンパイルエラーになることを利用してSFINEを用いオーバーロード対象外にする。get_valueの内部実装に依存しているため、それに変更があった場合は修正する必要がある。
		*/
		template<typename T>
		auto ReflectivePtreeConverter(T& dst, const boost::property_tree::ptree& src) -> decltype(std::cin >> dst, std::declval<typename boost::disable_if<std::is_base_of<Reflectable, T>>::type>());
		/*! Reflectableを継承した型へのPtree変換関数*/
		template<typename T>
		auto ReflectivePtreeConverter(Reflectable& dst, const boost::property_tree::ptree& src);
		/*! std::tupleへのPtree変換関数*/
		template<typename... Ts>
		void ReflectivePtreeConverter(std::tuple<Ts...>& dst, const boost::property_tree::ptree& src);

		/*! 配列型へのPtree変換関数*/
		PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEC(std::vector);
		PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEC(std::list);
		PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEC(std::deque);
		/*! セット型へのPtree変換関数*/
		PE_PTREE_CONVERT_TO_SET_TYPE_DEC(std::set);
		PE_PTREE_CONVERT_TO_SET_TYPE_DEC(std::unordered_set);
		/*! マップ型へのPtree変換関数*/
		PE_PTREE_CONVERT_TO_MAP_TYPE_DEC(std::map);
		PE_PTREE_CONVERT_TO_MAP_TYPE_DEC(std::unordered_map);

	}

	namespace private_ {
		template<typename T, typename... Rest>
		void ReflectivePtreeConverterError() {
			throw reflection_error(util::ConvertAndConnectToString("Ptreeからの変換に対応していない型\"", typeid(T).name(), "\"が指定されました。"));
		}

		template<size_t idx>
		void ReflectivePtreeConverterToStdTuple(std::tuple<>& dst, const std::vector<const boost::property_tree::ptree*>& src) {}
		template<size_t idx, typename F, typename... R>
		void ReflectivePtreeConverterToStdTuple(std::tuple<F, R...>& dst, const std::vector<const boost::property_tree::ptree*>& src) {
			F f{};
			planeta::util::ReflectivePtreeConverter(f, *src[idx]);
			std::tuple<R...> rtuple;
			ReflectivePtreeConverterToStdTuple<idx + 1, R...>(rtuple, src);
			dst = std::tuple_cat(std::make_tuple<F>(std::move(f)), rtuple);
		}
	}

	namespace util{
		template<typename... Ts>
		void ReflectivePtreeConverter(Ts...) {
			planeta::private_::ReflectivePtreeConverterError<Ts...>();
		}

		template<typename T>
		auto ReflectivePtreeConverter(T& dst, const boost::property_tree::ptree& src) -> decltype(std::cin >> dst, std::declval<typename boost::disable_if<std::is_base_of<Reflectable, T>, void>::type>()) {
			try {
				dst = src.get_value<T>();
			} catch (boost::property_tree::ptree_bad_data& e) {
				throw reflection_error(util::ConvertAndConnectToString("Ptreeから型\"", typeid(T).name(), "\"への変換に失敗しました。(", e.what(), ")"));
			}
		}

		template<typename T>
		auto ReflectivePtreeConverter(Reflectable& dst, const boost::property_tree::ptree& src) {
			try {
				ReflectivePtreeConverterFromReflectionSystem(dst, src);
			} catch (reflection_error& e) {
				throw reflection_error(util::ConvertAndConnectToString("Ptreeから型\"", typeid(T).name(), "\"への変換に失敗しました。(", e.what(), ")"));
			}
		}

		template<typename... Ts>
		void ReflectivePtreeConverter(std::tuple<Ts...>& dst, const boost::property_tree::ptree& src) {
			std::vector<const boost::property_tree::ptree*> ptree_vec;
			for (auto&& pp : src) {
				if (pp.first.empty() == false) {
					throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("std::tupleのPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}
				ptree_vec.emplace_back(&(pp.second));
			}
			if (sizeof...(Ts) != ptree_vec.size()) {
				throw reflection_error(ConvertAndConnectToString("要素数が", ptree_vec.size(), "ですが、対象のstd::tupleの要素数は", sizeof...(Ts), "です。"));
			}
			planeta::private_::ReflectivePtreeConverterToStdTuple<0, Ts...>(dst, ptree_vec);
		}

		PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEF(std::vector);
		PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEF(std::list);
		PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEF(std::deque);

		PE_PTREE_CONVERT_TO_SET_TYPE_DEF(std::set);
		PE_PTREE_CONVERT_TO_SET_TYPE_DEF(std::unordered_set);
		
		PE_PTREE_CONVERT_TO_MAP_TYPE_DEF(std::map);
		PE_PTREE_CONVERT_TO_MAP_TYPE_DEF(std::unordered_map);
	}
}

#undef PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEC
#undef PE_PTREE_CONVERT_TO_SET_TYPE_DEC
#undef PE_PTREE_CONVERT_TO_MAP_TYPE_DEC
#undef PE_PTREE_CONVERT_TO_ARRAY_TYPE_DEF
#undef PE_PTREE_CONVERT_TO_SET_TYPE_DEF
#undef PE_PTREE_CONVERT_TO_MAP_TYPE_DEF

//////////////////////////////////////////////////////////////////////////
//コピーハンドラ
//////////////////////////////////////////////////////////////////////////
namespace planeta {
	class Reflectable;
	namespace private_ {
		void ReflectiveCopyFromReflectionSystem(Reflectable& dst, const Reflectable& src);
	}
	namespace util {
		/*! コピー代入可能な型のコピーハンドラ*/
		template<typename T>
		auto ReflectiveCopyHandler(T& dst, const T& src) -> typename boost::enable_if<std::is_copy_assignable<T>, void>::type {
			dst = src;
		}
		/*! コピー代入不可能でReflectionAccessibleを継承している型*/
		template<typename T>
		auto ReflectiveCopyHandler(T& dst, const T& src) -> typename boost::enable_if_c<!std::is_copy_assignable_v<T>&&std::is_base_of_v<ReflectionAccessible, T>, void>::type {
			dst.ReflectiveCopyFrom(src);
		}
		/*! コピー代入不可能でReflectionAssignableを継承しておらず、Reflectableを継承している型*/
		template<typename T>
		auto ReflectiveCopyHandler(T& dst, const T& src) -> typename boost::enable_if_c<!std::is_copy_assignable_v<T>&&!std::is_base_of_v<ReflectionAccessible, T>&&std::is_base_of_v<Reflectable, T>, void>::type {
			planeta::private_::ReflectiveCopyFromReflectionSystem(dst, src);
		}
	}
}