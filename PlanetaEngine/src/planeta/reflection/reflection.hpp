/*Reflectionライブラリ
Version 1.0.0 2016/9/1
Version 1.0.1 2016/9/2 Reflectableクラスをコピームーブ可能に。RegisterObject時のエラー報告を初期化時に行うよう変更。
Version 1.0.2 2016/9/3 RE_REFLECTABLE_CLASSで、登録トリガーの無名名前空間変数が、翻訳空間ごとに作成されることが原因でクラスの重複登録エラーが起きていたのを修正。基底型を指定して登録することを可能に。
Version 1.0.3 2016/9/6 ReflectionUtility.hにostreamのインクルードを追加し、変換可能な型が変換できないバグを修正。プロパティ登録の補助マクロ追加。reflection_data_registererの引数型も確認するように変更。WriteOnlyPropertyのコンパイルエラーを修正。
Vertion 2.0.0 2016/9/29 LuaBind機能の追加。Reflectableのリフレクションアクセス機能をRefrectionAccessibleとReflectableClassAccessorに移譲し軽量化。単純な構造体でも、Reflectableクラスを継承しやすくなった。Ptree読み込みにおいて、Reflectableクラスの読み込み実装を変更。

@exception noexcept出ないクラスは、reflection_errorを投げる可能性がある

@todo Reflectableクラスがコピーできないことにより、変数、コピー対象にReflectableクラスを登録することにはほとんど対応していない。
現在、すべてのReflectableクラスはPtreeからの変換が可能なものとして登録されるので、登録時に未対応の設定を行えるようにする。
*/

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <type_traits>

#include "boost/core/enable_if.hpp"

#include "../core/string_utility.hpp"

#include "class_registerer_impl.hpp"

namespace plnt::reflection {
	class reflectable_class_accessor;
	class reflection_accessible;
	/*! @brief リフレクションシステム
		
		リフレクションシステムに静的関数でアクセスできるクラス。
	*/
	class reflection {
		friend reflectable_class_accessor;
		friend reflection_accessible;

	public:
		/*リフレクションシステムの初期化*/
		static void initialize();
		/*オブジェクトを登録*/
		template <class T>
		static void register_object(const std::string &object_type_id,
		                            std::unique_ptr<private_::class_info> &&class_info) {
			static_assert(std::is_base_of_v<reflectable, T> == true, "T must derive Reflectable.");
			//PE_LOG_MESSAGE("Object is registered.", id);
			register_object(typeid(T), object_type_id, std::move(class_info));
		}

		//! ObjectTypeIDでオブジェクトを作成
		static std::shared_ptr<reflectable> create_object_by_object_type_id(const std::string &object_type_id) noexcept;
		/*! @brief ObjectTypeIDでオブジェクトを作成して型変換
			@return 変換後のポインタを返す。存在しない場合はnullptrを返す。
		*/
		template <typename T>
		static std::shared_ptr<T> create_object_by_object_type_id(const std::string &object_type_id) noexcept {
			const auto ptr = create_object_by_object_type_id(object_type_id);
			if (ptr == nullptr) { return nullptr; }
			auto t_ptr = std::dynamic_pointer_cast<T>(ptr);
			if (t_ptr == nullptr) {
				out_put_error_log(util::convert_and_connect_to_string("指定型に変換できませんでした。(ObjectTypeID:", object_type_id,
				                                                  ",TargetType:", typeid(T).name(), ")"),
				                  __FUNCTION__);
				return nullptr;
			}
			return t_ptr;
		}

		//! 型情報でオブジェクトを作成
		static std::shared_ptr<reflectable> create_object_by_std_type_info(const std::type_info &t_info) noexcept;
		/*! @brief 型情報でオブジェクトを作成して型変換
			@return 変換後のポインタを返す。存在しない場合はnullptrを返す。
		*/
		template <typename T>
		static std::shared_ptr<T> create_object_by_std_type_info(const std::type_info &t_info) noexcept {
			const auto ptr = create_object_by_std_type_info(t_info);
			if (ptr == nullptr) { return nullptr; }
			auto t_ptr = std::dynamic_pointer_cast<T>(ptr);
			if (t_ptr == nullptr) {
				out_put_error_log(util::convert_and_connect_to_string("指定型に変換できませんでした。(std::type_info:", t_info.name(),
				                                                  ",TargetType:", typeid(T).name(), ")"),
				                  __FUNCTION__);
				return nullptr;
			}
			return t_ptr;
		}

		//! 型からObjectTypeIDを取得する
		template <typename T>
		static std::string get_object_type_id_by_type() { return get_object_type_id_by_std_type_info(typeid(T)); }

		//! std::type_infoからObjectTypeIDを取得する
		static std::string get_object_type_id_by_std_type_info(const std::type_info &t_info);
		//! ObjectTypeIDから型情報を取得する
		static const std::type_info &get_std_type_info_by_object_type_id(const std::string &id);
		//! リフレクションクラスへのアクセサを得る
		static std::shared_ptr<reflectable_class_accessor> get_reflectable_class_accessor(const std::type_info &ti);
		//! LuaStateに登録されているクラスを登録する
		static void bind_classes_to_lua(lua_State *l);
		//! 登録されたクラス数を取得
		static size_t get_registered_class_count() noexcept;

	private:
		reflection() noexcept;
		class impl;
		static impl &get_impl();
		static void register_object(const std::type_info &t_info, const std::string &id,
		                            std::unique_ptr<private_::class_info> &&class_info);
		//Gameクラスへのヘッダファイル依存をなくすためにこうする
		static void out_put_error_log(const std::string &detail, const std::string &place);

		//Reflectableに公開
		static const private_::class_info *get_class_info_reflectable(const std::type_info &t_info) noexcept;
	};

	namespace private_ {
		//抽象クラスでクリエータが指定された場合(ありえないので定義しない)
		/*template<class C>
		auto SetCreator(ClassInfo& ci, const std::function<std::shared_ptr<Reflectable>()>& creator) -> typename boost::enable_if<std::is_abstract<C>, void>::type {
			ci.is_abstract = true;
		}*/
		//抽象クラスでなくクリエータが指定された場合
		template <class C>
		auto set_creator(class_info &ci,
		                 const std::function<std::shared_ptr<reflectable>()> &creator) -> typename boost::disable_if<
			std::is_abstract<C>, void>::type {
			ci.is_abstract = false;
			ci.creator = creator;
		}

		//抽象クラスでクリエータが指定されていない場合
		template <class C>
		auto set_creator(class_info &ci) -> std::enable_if_t<std::is_abstract_v<C>, void> { ci.is_abstract = true; }

		//抽象クラスでなくクリエータが指定されていない場合
		template <class C>
		auto set_creator(class_info &ci) -> typename boost::disable_if<std::is_abstract<C>, void>::type {
			ci.is_abstract = false;
			ci.creator = [] { return std::make_shared<C>(); };
		}

		//reflection_data_registerer静的関数を持っている場合
		template <class C>
		auto register_reflection_data(
			class_info &ci) -> std::enable_if_t<has_reflection_data_registerer_v<C>, void> {
			class_registerer_impl<C> registerer(ci);
			registerer.begin_proc();
			C::reflection_data_registerer(registerer);
			registerer.end_proc();
		}

		//reflection_data_registerer静的関数を持っていない場合
		template <class C>
		auto register_reflection_data(
			class_info &) -> typename boost::disable_if<has_reflection_data_registerer<C>, void>::type {
			//何もしない
		}

		//superエイリアスを持っているとき
		template <class C>
		auto register_type_info(class_info &ci) -> std::enable_if_t<has_super_alias_v<C>, void> {
			ci.this_t_info = typeid(C);
			ci.super_t_info = typeid(C::super);
		}

		//superエイリアスを持っていないとき(Reflectableを親とする)
		template <class C>
		auto register_type_info(class_info &ci) -> typename boost::disable_if<has_super_alias<C>, void>::type {
			ci.this_t_info = typeid(C);
			ci.super_t_info = typeid(reflectable);
		}

		//superエイリアスを持っていて基底クラスの型情報が指定されたとき
		template <class C>
		auto register_type_info(class_info &ci,
		                        const std::type_info &sti) -> std::enable_if_t<has_super_alias_v<C>, void> {
			ci.this_t_info = typeid(C);
			ci.super_t_info = sti;
		}

		//superエイリアスを持っていなくて基底クラスの型情報が指定されたとき
		template <class C>
		auto register_type_info(class_info &ci,
		                        const std::type_info &sti) -> typename boost::disable_if<
			has_super_alias<C>, void>::type {
			ci.this_t_info = typeid(C);
			ci.super_t_info = sti;
		}

		template <typename C>
		class class_register_trigger {
		public:
			//オブジェクトタイプIDとクリエータを指定
			class_register_trigger(const char *object_id,
			                       const std::function<std::shared_ptr<reflectable>()> &creator) {
				auto ci = std::make_unique<class_info>();
				set_creator<C>(*ci, creator);
				register_reflection_data<C>(*ci);
				register_type_info<C>(*ci);
				reflection::register_object<C>(object_id, std::move(ci));
			}

			//オブジェクトタイプIDを指定
			explicit class_register_trigger(const char *object_id) {
				auto ci = std::make_unique<class_info>();
				set_creator<C>(*ci);
				register_reflection_data<C>(*ci);
				register_type_info<C>(*ci);
				reflection::register_object<C>(object_id, std::move(ci));
			}

			//オブジェクトタイプIDとクリエータ、基底クラスの型情報を指定
			class_register_trigger(const char *object_id, const std::function<std::shared_ptr<reflectable>()> &creator,
			                       const std::type_info &sti) {
				auto ci = std::make_unique<class_info>();
				set_creator<C>(*ci, creator);
				register_reflection_data<C>(*ci);
				register_type_info<C>(*ci, sti);
				reflection::register_object<C>(object_id, std::move(ci));
			}

			//オブジェクトタイプID、基底クラスの型情報を指定
			class_register_trigger(const char *object_id, const std::type_info &sti) {
				auto ci = std::make_unique<class_info>();
				set_creator<C>(*ci);
				register_reflection_data<C>(*ci);
				register_type_info<C>(*ci, sti);
				reflection::register_object<C>(object_id, std::move(ci));
			}
		};
	}
}

namespace plnt::reflection::private_ {
	//メンバクラス内静的変数がクラス単位で生成されることを利用して、複数回呼ばれても最初の一回しか登録処理を行わないようにする。
	template <typename T>
	struct reflectable_class_register_helper {
		template <typename... Params>
		explicit reflectable_class_register_helper(Params &&... params) {
			static class_register_trigger<T> class_register_trigger{params...};
		}
	};
}

/*! @def
	クラスをリフレクションシステムに登録する(登録する型は、公開型エイリアスsuperが定義されていること)
	@param type 型
*/
#define PE_REFLECTABLE_CLASS(type)\
namespace { ::plnt::reflection::private_::reflectable_class_register_helper<type> pe_reflectable_class_register_helper_##type##_ = ::plnt::reflection::private_::reflectable_class_register_helper<type>{#type}; }

/*! @def
IDを指定してクラスをリフレクションシステムに登録する(登録する型は、公開型エイリアスsuperが定義されていること)
@param type 型
*/
#define PE_REFLECTABLE_CLASS_WITH_ID(type, id)\
namespace { ::plnt::reflection::private_::reflectable_class_register_helper<type> pe_reflectable_class_register_helper_##id##_ = ::plnt::reflection::private_::reflectable_class_register_helper<type>{#id}; }

/*! @def
	親クラスを指定してクラスをリフレクションシステムに登録する(登録する型に公開型エイリアスsuperは定義されている必要はない。定義されていても参照されない)
	@param type 型
*/
#define PE_REFLECTABLE_CLASS_SPECIFY_SUPER(type, super)\
namespace { ::plnt::reflection::private_::reflectable_class_register_helper<type> pe_reflectable_class_register_helper_##type##_ = ::plnt::reflection::private_::reflectable_class_register_helper<type>{#type, typeid(super)}; }

/*! @def
クラスをクリエータを指定してリフレクションシステムに登録する(登録する型は、公開型エイリアスsuperが定義されていること)
@param type 型
@param creator クリエータ(std::shared_ptr<Object>()の関数型)
*/
#define PE_REFLECTABLE_CLASS_WITH_CREATOR(type, creator)\
namespace { ::plnt::reflection::private_::reflectable_class_register_helper<type> pe_reflectable_class_register_helper_##type##_ = ::plnt::reflection::private_::reflectable_class_register_helper<type>{#type, creator}; }
