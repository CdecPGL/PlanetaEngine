#pragma once

#include <string>
#include <typeinfo>
#include <memory>

#include "planeta/reflection/reflectable.hpp"

namespace plnt {
	/*! @brief 全てのクラスの基底となる基本クラス

		型情報管理、リフレクションシステムなどを提供する。これらの機能が必要ない場合は継承する必要はない
	*/
	class Object : public reflection::reflectable {
	public:
		using super = reflectable;
		//! デフォルトコンストラクタ
		Object() = default;
		//! デストラクタ
		~Object() override = default;

		//! 文字列化する。継承することで独自の文字列か機能を追加可能。
		[[nodiscard]] virtual std::string to_string() const { return typeid(*this).name(); }

		auto operator<=>(const Object &) const = default;
	};

	PE_REFLECTABLE_CLASS(Object);
}
