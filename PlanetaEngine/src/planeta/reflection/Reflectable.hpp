#pragma once

#include "ClassRegisterer.hpp"
#include "Reflection.hpp"

namespace plnt::reflection {
	/*! Reflection基底クラス。レフレクションに対応するクラスは継承しなければならない。
	*/
	class Reflectable {
	public:
		virtual ~Reflectable() = 0 {};
	};
}

/*! リフレクションデータ登録関数の宣言を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DECLARATION(type)\
public:\
static void ReflectionDataRegisterer(::plnt::reflection::ClassRegisterer<type>& registerer);\

/*! リフレクションデータ登録関数の定義を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DEFINITION(type)\
void type::ReflectionDataRegisterer(::plnt::reflection::ClassRegisterer<type>& registerer)

/*! リフレクションデータ登録関数の宣言定義を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DECL_DEF(type)\
public:\
static void ReflectionDataRegisterer(::plnt::reflection::ClassRegisterer<type>& registerer)
