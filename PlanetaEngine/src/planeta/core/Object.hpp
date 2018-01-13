#pragma once
#include <string>
#include <typeinfo>
#include <memory>
#include "planeta/reflection/Reflectable.hpp"

#undef max //windows.hで定義されているmaxマクロを無効化(std::numeric_limits<size_t>::max()のため)

namespace plnt {
	/*! @brief 全てのクラスの基底となる基本クラス

		型情報管理、リフレクションシステムなどを提供する。これらの機能が必要ない場合は継承する必要はない
	*/
	class Object : public reflection::Reflectable {
	public:
		using Super = Reflectable;
		//! デフォルトコンストラクタ
		Object() = default;
		//! デストラクタ
		virtual ~Object() = default;
		//! インスタンスが同じか確認
		bool Equals(const Object* o)const { return o == this; }
		//! インスタンスが同じか確認
		bool Equals(const Object& o)const { return &o == this; }
		//! 文字列化する。継承することで独自の文字列か機能を追加可能。
		virtual std::string ToString()const { return typeid(*this).name(); }
	};
	PE_REFLECTABLE_CLASS(Object);
}

//template<typename T>
//struct A {
//	static planeta::private_::ClassRegisterTrigger<planeta::core::Object> globalobject_registerer;
//};
//template<typename T>
//planeta::private_::ClassRegisterTrigger<planeta::core::Object> A<T>::globalobject_registerer = {""};
//template struct A<int>;