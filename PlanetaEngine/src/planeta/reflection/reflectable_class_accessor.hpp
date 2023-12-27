#pragma once

#include <memory>
#include <string>

#include "boost/property_tree/ptree.hpp"

#include "planeta/reflection/reflection.hpp"

namespace plnt::reflection {
	class reflectable;

	namespace private_ {
		struct class_info;
	}

	class reflectable_class_accessor final {
	public:
		using super = void;
		explicit reflectable_class_accessor(const private_::class_info *ci) noexcept;
		reflectable_class_accessor(const reflectable_class_accessor &obj) = delete;
		reflectable_class_accessor(reflectable_class_accessor &&obj) = delete;
		~reflectable_class_accessor() noexcept;
		reflectable_class_accessor &operator=(const reflectable_class_accessor &obj) = delete;
		reflectable_class_accessor &operator=(reflectable_class_accessor &&obj) = delete;
		//! 変数またはプロパティを変更する
		template <typename T>
		void set_variable(reflectable &obj, const std::string &var_id, const T &v) { SetVariable_(var_id, v); }

		/*!
		@brief 変数またはプロパティを取得する
		@todo コピー不可能な型を取得できない
		*/
		template <typename T>
		void get_variable(reflectable &obj, const std::string &var_id, T &v) {
			boost::any av;
			get_variable(obj, var_id, av);
			try { v = boost::any_cast<T>(av); } catch (boost::bad_any_cast &) {
				std::string obj_tid = reflection::get_object_type_id_by_std_type_info(typeid(*this));
				throw reflection_error(util::ConvertAndConnectToString("クラス\"", obj_tid, "\"の変数またはプロパティ\"", var_id,
				                                                       "\"の書き込みにおいて型の不一致エラーが発生しました。(変数型:",
				                                                       av.type().name(), ", 指定型:", typeid(T).name(),
				                                                       ")"));
			}
		}

		//! クローンを作成する
		std::shared_ptr<reflectable> clone(reflectable &obj) const;
		//! コピーする(未実装)
		void copy(reflectable &dst, const reflectable &src) const;
		//! boost::ptreeからデータの読み込み
		void load_from_ptree(reflectable &obj, const boost::property_tree::ptree &pt) const;

	private:
		class impl;
		std::unique_ptr<impl> impl_;
		void set_variable(reflectable &obj, const std::string &var_id, const boost::any &v) const;
		void get_variable(reflectable &obj, const std::string &var_id, boost::any &v) const;
	};
}
