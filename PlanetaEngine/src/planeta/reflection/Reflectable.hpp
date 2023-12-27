#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "reflection.hpp"

namespace plnt::reflection {
	/*! reflection基底クラス。レフレクションに対応するクラスは継承しなければならない。
	*/
	class reflectable {
	public:
		reflectable() = default;
		reflectable(const reflectable &) = default;
		reflectable(reflectable &&) = default;
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition, modernize-use-equals-default)
		virtual ~reflectable() = 0 { }

		reflectable &operator=(const reflectable &) = default;
		reflectable &operator=(reflectable &&) = default;
	};
}

/*! リフレクションデータ登録関数の宣言を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DECLARATION(type)\
public:\
static void reflection_data_registerer(::plnt::reflection::class_registerer<type>& registerer);

/*! リフレクションデータ登録関数の定義を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DEFINITION(type)\
void type::reflection_data_registerer(::plnt::reflection::class_registerer<type> &registerer)

/*! リフレクションデータ登録関数の宣言定義を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DECL_DEF(type)\
public:\
static void reflection_data_registerer(::plnt::reflection::class_registerer<type>& registerer)
