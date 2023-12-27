#pragma once

#include <memory>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"
#include "planeta/reflection/reflection.hpp"

namespace plnt::reflection {
	class reflectable;
	/*! Reflection基底クラス。レフレクションに対応するクラスは継承しなければならない。
	*/
	class reflection_accessible {
	public:
		using super = void;
		reflection_accessible() noexcept;
		reflection_accessible(const reflection_accessible &obj) noexcept;
		reflection_accessible(reflection_accessible &&obj) noexcept;
		virtual ~reflection_accessible() noexcept = 0;
		reflection_accessible &operator=(const reflection_accessible &obj) noexcept;
		reflection_accessible &operator=(reflection_accessible &&obj) noexcept;
		//! 変数またはプロパティを変更する
		template <typename T>
		void reflective_set_variable(const std::string &var_id, const T &v) { SetVariable_(var_id, v); }

		/*!
		@brief 変数またはプロパティを取得する
		@todo コピー不可能な型を取得できない
		*/
		template <typename T>
		void reflective_get_variable(const std::string &var_id, T &v) {
			boost::any av;
			get_variable(var_id, av);
			try { v = boost::any_cast<T>(av); } catch (boost::bad_any_cast &) {
				std::string obj_tid = reflection::get_object_type_id_by_std_type_info(typeid(*this));
				throw reflection_error(util::ConvertAndConnectToString("クラス\"", obj_tid, "\"の変数またはプロパティ\"", var_id,
				                                                       "\"の書き込みにおいて型の不一致エラーが発生しました。(変数型:",
				                                                       av.type().name(), ", 指定型:", typeid(T).name(),
				                                                       ")"));
			}
		}

		//! クローンを作成する
		[[nodiscard]] std::shared_ptr<reflectable> reflective_clone() const;
		//! コピーする(未実装)
		void reflective_copy_from(const reflectable &src) const;
		//! boost::ptreeからデータの読み込み
		void reflective_load_from_ptree(const boost::property_tree::ptree &pt) const;

	private:
		class impl;
		std::unique_ptr<impl> impl_;
		void set_variable(const std::string &var_id, const boost::any &v) const;
		void get_variable(const std::string &var_id, boost::any &v) const;
	};
}
