#pragma once

#include <memory>
#include <string>

#include "boost/property_tree/ptree.hpp"

namespace plnt {
	class Reflectable;
	namespace private_ {
		struct ClassInfo;
	}
	class ReflectableClassAccessor final {
	public:
		using Super = void;
		ReflectableClassAccessor(const private_::ClassInfo* ci)noexcept;
		ReflectableClassAccessor(const ReflectableClassAccessor& obj) = delete;
		ReflectableClassAccessor(ReflectableClassAccessor&& obj) = delete;
		virtual ~ReflectableClassAccessor()noexcept;
		ReflectableClassAccessor& operator=(const ReflectableClassAccessor& obj) = delete;
		ReflectableClassAccessor& operator=(ReflectableClassAccessor&& obj) = delete;
		//! 変数またはプロパティを変更する
		template<typename T>
		void SetVariable(Reflectable& obj, const std::string& var_id, const T& v) {
			SetVariable_(var_id, v);
		}
		/*!
		@brief 変数またはプロパティを取得する
		@todo コピー不可能な型を取得できない
		*/
		template<typename T>
		void GetVariable(Reflectable& obj, const std::string& var_id, T& v) {
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
		std::shared_ptr<Reflectable> Clone(Reflectable& obj);
		//! コピーする(未実装)
		void Copy(Reflectable& dst, const Reflectable& src);
		//! boost::ptreeからデータの読み込み
		void LoadFromPtree(Reflectable& obj, const boost::property_tree::ptree& pt);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		void SetVariable_(Reflectable& obj, const std::string& var_id, const boost::any& v);
		void GetVariable_(Reflectable& obj, const std::string& var_id, boost::any& v);
	};
}