#pragma once

#include <memory>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"

#include "Reflection.h"

namespace planeta {
	/*! Reflection基底クラス。レフレクションに対応するクラスは継承しなければならない。
	*/
	class Reflectable {
	public:
		using Super = void;
		Reflectable()noexcept;
		Reflectable(const Reflectable& obj)noexcept;
		Reflectable(Reflectable&& obj)noexcept;
		virtual ~Reflectable()noexcept;
		Reflectable& operator=(const Reflectable& obj)noexcept;
		Reflectable& operator=(Reflectable&& obj)noexcept;
		//! 変数またはプロパティを変更する
		template<typename T>
		void SetVariable(const std::string& var_id, const T& v) {
			SetVariable_(var_id, v);
		}
		/*!
		@brief 変数またはプロパティを取得する
		@todo コピー不可能な型を取得できない
		*/
		template<typename T>
		void GetVariable(const std::string& var_id, T& v) {
			boost::any av;
			GetVariable_(var_id, av);
			try {
				v = boost::any_cast<T>(av);
			} catch (boost::bad_any_cast&) {
				std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(*this));
				throw reflection_error(util::ConvertAndConnectToString("クラス\"", obj_tid, "\"の変数またはプロパティ\"", var_id, "\"の書き込みにおいて型の不一致エラーが発生しました。(変数型:", av.type().name(), ", 指定型:", typeid(T).name(), ")"));
			}
		}
		//! クローンを作成する
		std::shared_ptr<Reflectable> Clone();
		//! コピーする(未実装)
		void CopyFrom(const Reflectable& src);
		//! boost::ptreeからデータの読み込み
		void LoadFromPtree(const boost::property_tree::ptree& pt);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		void SetVariable_(const std::string& var_id, const boost::any& v);
		void GetVariable_(const std::string& var_id, boost::any& v);
	};
}

/*! リフレクションデータ登録関数の宣言を行うマクロ*/
#define PE_REFLECTION_DATA_REGISTERER_DECLARATION(type)\
public:\
static void ReflectionDataRegisterer(ClassRegisterer<type>& registerer);\
private:

