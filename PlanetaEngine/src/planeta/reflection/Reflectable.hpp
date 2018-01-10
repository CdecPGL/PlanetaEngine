#pragma once

#include "planeta/reflection/ClassRegisterer.hpp"
#include "planeta/reflection/Reflection.hpp"

namespace plnt {
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
static void ReflectionDataRegisterer(::plnt::ClassRegisterer<type>& registerer);\

/*! リフレクションデータ登録関数の定義を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DEFINITION(type)\
void type::ReflectionDataRegisterer(::plnt::ClassRegisterer<type>& registerer)

/*! リフレクションデータ登録関数の宣言定義を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DECL_DEF(type)\
public:\
static void ReflectionDataRegisterer(::plnt::ClassRegisterer<type>& registerer)
