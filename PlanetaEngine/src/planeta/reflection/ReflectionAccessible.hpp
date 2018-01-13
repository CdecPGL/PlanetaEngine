#pragma once

#include <memory>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"

namespace plnt::reflection {
	class Reflectable;
	/*! Reflection基底クラス。レフレクションに対応するクラスは継承しなければならない。
	*/
	class ReflectionAccessible {
	public:
		using Super = void;
		ReflectionAccessible()noexcept;
		ReflectionAccessible(const ReflectionAccessible& obj)noexcept;
		ReflectionAccessible(ReflectionAccessible&& obj)noexcept;
		virtual ~ReflectionAccessible()noexcept = 0;
		ReflectionAccessible& operator=(const ReflectionAccessible& obj)noexcept;
		ReflectionAccessible& operator=(ReflectionAccessible&& obj)noexcept;
		//! 変数またはプロパティを変更する
		template<typename T>
		void ReflectiveSetVariable(const std::string& var_id, const T& v) {
			SetVariable_(var_id, v);
		}
		/*!
		@brief 変数またはプロパティを取得する
		@todo コピー不可能な型を取得できない
		*/
		template<typename T>
		void ReflectiveGetVariable(const std::string& var_id, T& v) {
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
		std::shared_ptr<Reflectable> ReflectiveClone();
		//! コピーする(未実装)
		void ReflectiveCopyFrom(const Reflectable& src);
		//! boost::ptreeからデータの読み込み
		void ReflectiveLoadFromPtree(const boost::property_tree::ptree& pt);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		void SetVariable_(const std::string& var_id, const boost::any& v);
		void GetVariable_(const std::string& var_id, boost::any& v);
	};
}

