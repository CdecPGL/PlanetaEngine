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

#include "MetaprogrammingUtility.hpp"

namespace planeta {
	class Reflectable;
	class ReflectionAccessible;
	template<class C>
	class ClassRegisterer;
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

//////////////////////////////////////////////////////////////////////////
//宣言短縮用マクロ
//////////////////////////////////////////////////////////////////////////
#define PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEC(array_type)\
template<typename T, typename... Rest>\
struct ReflectivePtreeConverter_Layer0 <array_type<T, Rest...>> {\
	static void Convert(array_type<T, Rest...>& dst, const boost::property_tree::ptree& src);\
};
#define PE_REFLECTIVE_CONVERTER_LAYER0_SET_TYPE_DEC(set_type)\
template<typename T, typename... Rest>\
struct ReflectivePtreeConverter_Layer0 <set_type<T, Rest...>> {\
	static void Convert(set_type<T, Rest...>& dst, const boost::property_tree::ptree& src);\
};
#define PE_REFLECTIVE_CONVERTER_LAYER0_MAP_TYPE_DEC(map_type)\
template<typename T, typename... Rest>\
struct ReflectivePtreeConverter_Layer0 <map_type<std::string, T, Rest...>> {\
	static void Convert(map_type<std::string, T, Rest...>& dst, const boost::property_tree::ptree& src);\
};
//////////////////////////////////////////////////////////////////////////
//定義短縮用マクロ
//////////////////////////////////////////////////////////////////////////
#define PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEF(array_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter_Layer0<array_type<T, Rest...>>::Convert(array_type<T, Rest...>& dst, const boost::property_tree::ptree& src) {\
	for (auto&& pp : src) {\
		if (pp.first.empty() == false) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("配列型のPtreeキーは空である必要があります。(読み取られたキー:",pp.first,")"));\
		}\
		T dat{};\
		planeta::util::ReflectivePtreeConverter(dat, pp.second);\
		dst.push_back(std::move(dat));\
	}\
}
#define PE_REFLECTIVE_CONVERTER_LAYER0_SET_TYPE_DEF(set_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter_Layer0<set_type<T, Rest...>>::Convert(set_type<T, Rest...>& dst, const boost::property_tree::ptree& src) {\
	for (auto&& pp : src) {\
		if (pp.first.empty() == false) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("配列型のPtreeキーは空である必要があります。(読み取られたキー:",pp.first,")"));\
		}\
		T dat{};\
		planeta::util::ReflectivePtreeConverter(dat, pp.second);\
		dst.insert(std::move(dat));\
	}\
}
#define PE_REFLECTIVE_CONVERTER_LAYER0_MAP_TYPE_DEF(map_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter_Layer0 <map_type<std::string, T, Rest...>>::Convert(map_type<std::string, T, Rest...>& dst, const boost::property_tree::ptree& src){\
	for (auto&& pp : src) {\
		if (pp.first.empty() == true) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("マップ型のPtreeキーは空であってはいけません。"));\
		}\
		T dat{};\
		planeta::util::ReflectivePtreeConverter(dat, pp.second);\
		dst.emplace(pp.first, std::move(dat));\
	}\
}

namespace planeta {
	//////////////////////////////////////////////////////////////////////////
	//宣言
	//////////////////////////////////////////////////////////////////////////
	namespace private_ {
		template<typename T, typename... Rest>
		void ReflectivePtreeConverterError();
		void ReflectivePtreeConverterFromReflectionSystem(Reflectable& dst, const boost::property_tree::ptree& src);
		template<size_t idx>
		void ReflectivePtreeConverterToStdTuple(std::tuple<>& dst, const std::vector<const boost::property_tree::ptree*>& src);
		template<size_t idx, typename F, typename... R>
		void ReflectivePtreeConverterToStdTuple(std::tuple<F, R...>& dst, const std::vector<const boost::property_tree::ptree*>& src);

		//Ptreeからの変換不可能(Layer3)
		template<typename T>
		struct ReflectivePtreeConverter_Layer3 {
			static void Convert(T& dst, const boost::property_tree::ptree& src);
		};

		/*! @brief ptreeから直接変換可能な型へのPtree変換関数(Layer2)
		@note BoostLibrary1.64.0では、get_value内でistreamによる型の変換を行っている。これを利用して、get_valueに対応していない型に対しては、std::cinでもコンパイルエラーになることを利用してSFINEを用いオーバーロード対象外にする。get_valueの内部実装に依存しているため、それに変更があった場合は修正する必要がある。
		*/
		//Ptreeから直接変換可能
		template<typename T, typename U = void>
		struct ReflectivePtreeConverter_Layer2 {
			static void Convert(T& dst, const boost::property_tree::ptree& src);
		};
		//直接変換不可能
		template<typename T>
		struct ReflectivePtreeConverter_Layer2<T, std::enable_if_t<mp_util::IsIStreamCompatible_v<T>>> {
			static void Convert(T& dst, const boost::property_tree::ptree& src);
		};

		//Reflectableの継承による分岐(Layer1)
		//非継承
		template<typename T, typename U=void>
		struct ReflectivePtreeConverter_Layer1 {
			static void Convert(T& dst, const boost::property_tree::ptree& src);
		};
		//継承
		template<typename T>
		struct ReflectivePtreeConverter_Layer1<T, std::enable_if_t<std::is_base_of_v<planeta::Reflectable, T>>> {
			static void Convert(T& dst, const boost::property_tree::ptree& src);
		};

		//特殊型への分岐(Layer0)
		//非特殊型
		template<typename T>
		struct ReflectivePtreeConverter_Layer0 {
			static void Convert(T& dst, const boost::property_tree::ptree& src);
		};
		//std::tuple
		template<typename... Ts>
		struct ReflectivePtreeConverter_Layer0<std::tuple<Ts...>> {
			static void Convert(std::tuple<Ts...>& dst, const boost::property_tree::ptree& src);
		};
		//std::vector
		PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEC(std::vector);
		PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEC(std::list);
		PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEC(std::deque);
		//std::set
		PE_REFLECTIVE_CONVERTER_LAYER0_SET_TYPE_DEC(std::set);
		PE_REFLECTIVE_CONVERTER_LAYER0_SET_TYPE_DEC(std::unordered_set);
		//std::map
		PE_REFLECTIVE_CONVERTER_LAYER0_MAP_TYPE_DEC(std::map);
		PE_REFLECTIVE_CONVERTER_LAYER0_MAP_TYPE_DEC(std::unordered_map);
	}

	namespace util {
		template<typename T>
		void ReflectivePtreeConverter(T& dst, const boost::property_tree::ptree& src);
	}
	//////////////////////////////////////////////////////////////////////////
	//定義
	//////////////////////////////////////////////////////////////////////////
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

		//Layer3
		template<typename T>
		void ReflectivePtreeConverter_Layer3<T>::Convert(T& dst, const boost::property_tree::ptree& src) {
			planeta::private_::ReflectivePtreeConverterError<T>();
		}
		//Layer2
		template<typename T, typename U = void>
		void ReflectivePtreeConverter_Layer2<T, U>::Convert(T& dst, const boost::property_tree::ptree& src) {
			ReflectivePtreeConverter_Layer3<T>::Convert(dst, src);
		}
		template<typename T>
		void ReflectivePtreeConverter_Layer2<T, std::enable_if_t<mp_util::IsIStreamCompatible_v<T>>>::Convert(T& dst, const boost::property_tree::ptree& src) {
			try {
				dst = src.get_value<T>();
			}
			catch (boost::property_tree::ptree_bad_data& e) {
				throw reflection_error(util::ConvertAndConnectToString("Ptreeから型\"", typeid(T).name(), "\"への変換に失敗しました。(", e.what(), ")"));
			}
		}
		//Layer1
		template<typename T, typename U = void>
		void ReflectivePtreeConverter_Layer1<T, U>::Convert(T& dst, const boost::property_tree::ptree& src) {
			ReflectivePtreeConverter_Layer2<T>::Convert(dst, src);
		}
		template<typename T>
		void ReflectivePtreeConverter_Layer1<T, std::enable_if_t<std::is_base_of_v<planeta::Reflectable, T>>>::Convert(T& dst, const boost::property_tree::ptree& src) {
			try {
				planeta::private_::ReflectivePtreeConverterFromReflectionSystem(dst, src);
			}
			catch (reflection_error& e) {
				throw planeta::reflection_error(util::ConvertAndConnectToString("Ptreeから型\"", typeid(T).name(), "\"への変換に失敗しました。(", e.what(), ")"));
			}
		};
		//Layer0
		template<typename T>
		void ReflectivePtreeConverter_Layer0<T>::Convert(T& dst, const boost::property_tree::ptree& src) {
			ReflectivePtreeConverter_Layer1<T>::Convert(dst, src);
		}
		template<typename... Ts>
		void ReflectivePtreeConverter_Layer0<std::tuple<Ts...>>::Convert(std::tuple<Ts...>& dst, const boost::property_tree::ptree& src) {
			std::vector<const boost::property_tree::ptree*> ptree_vec;
			for (auto&& pp : src) {
				if (pp.first.empty() == false) {
					throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("std::tupleのPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}
				ptree_vec.emplace_back(&(pp.second));
			}
			if (sizeof...(Ts) != ptree_vec.size()) {
				throw reflection_error(planeta::util::ConvertAndConnectToString("要素数が", ptree_vec.size(), "ですが、対象のstd::tupleの要素数は", sizeof...(Ts), "です。"));
			}
			planeta::private_::ReflectivePtreeConverterToStdTuple<0, Ts...>(dst, ptree_vec);
		}
		//std::vector
		PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEF(std::vector);
		PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEF(std::list);
		PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEF(std::deque);
		//std::set
		PE_REFLECTIVE_CONVERTER_LAYER0_SET_TYPE_DEF(std::set);
		PE_REFLECTIVE_CONVERTER_LAYER0_SET_TYPE_DEF(std::unordered_set);
		//std::map
		PE_REFLECTIVE_CONVERTER_LAYER0_MAP_TYPE_DEF(std::map);
		PE_REFLECTIVE_CONVERTER_LAYER0_MAP_TYPE_DEF(std::unordered_map);
	}

	namespace util {
		template<typename T>
		void ReflectivePtreeConverter(T& dst, const boost::property_tree::ptree& src) {
			private_::ReflectivePtreeConverter_Layer0<T>::Convert(dst, src);
		}
	}
}

#undef PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEC
#undef PE_REFLECTIVE_CONVERTER_LAYER0_SET_TYPE_DEC
#undef PE_REFLECTIVE_CONVERTER_LAYER0_MAP_TYPE_DEC
#undef PE_REFLECTIVE_CONVERTER_LAYER0_ARRAY_TYPE_DEF
#undef PE_REFLECTIVE_CONVERTER_LAYER0_SET_TYPE_DEF
#undef PE_REFLECTIVE_CONVERTER_LAYER0_MAP_TYPE_DEF

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