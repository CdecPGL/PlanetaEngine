/*Reflectionライブラリ
Version 1.0.0 2016/9/1
Version 1.0.1 2016/9/2 Reflectableクラスをコピームーブ可能に。RegisterObject時のエラー報告を初期化時に行うよう変更。
Version 1.0.2 2016/9/3 RE_REFLECTABLE_CLASSで、登録トリガーの無名名前空間変数が、翻訳空間ごとに作成されることが原因でクラスの重複登録エラーが起きていたのを修正。

@exception noexcept出ないクラスは、reflection_errorを投げる可能性がある

@todo Reflectableクラスがコピーできないことにより、変数、コピー対象にReflectableクラスを登録することにはほとんど対応していない。
現在、すべてのReflectableクラスはPtreeからの変換が可能なものとして登録されるので、登録時に未対応の設定を行えるようにする。
*/

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <type_traits>
#include "SystemLog.h"
#include "ClassRegisterer.h"
#include "ReflectionExceptions.h"

namespace planeta {
	class Reflectable;
	/*! @brief リフレクションシステム
		
		リフレクションシステムに静的関数でアクセスできるクラス。
	*/
	class Reflection{
		friend Reflectable;
	public:
		/*リフレクションシステムの初期化*/
		static void Initialize();
		/*オブジェクトを登録*/
		template<class T>
		static void RegisterObject(const std::string& object_type_id, std::unique_ptr<private_::ClassInfo>&& class_info) {
			static_assert(std::is_base_of<Reflectable, T>::value == true, "T must derive Reflectable.");
			//PE_LOG_MESSAGE("Object is registered.", id);
			class_info->this_t_info = typeid(T);
			class_info->super_t_info = typeid(T::Super);
			RegisterObject_(typeid(T), object_type_id, std::move(class_info));
		}
		//! ObjectTypeIDでオブジェクトを作成
		static std::shared_ptr<Reflectable> CreateObjectByObjectTypeID(const std::string& object_type_id)noexcept;
		/*! @brief ObjectTypeIDでオブジェクトを作成して型変換
			@return 変換後のポインタを返す。存在しない場合はnullptrを返す。
		*/
		template<typename T>
		static std::shared_ptr<T> CreateObjectByObjectTypeID(const std::string& object_type_id)noexcept {
			auto ptr = CreateObjectByObjectTypeID(object_type_id);
			if (ptr == nullptr) { return nullptr; }
			auto tptr = std::dynamic_pointer_cast<T>(ptr);
			if (tptr == nullptr) {
				PE_LOG_ERROR("指定型に変換できませんでした。(ObjectTypeID:", object_type_id, ",TargetType:", typeid(T).name(), ")");
				return nullptr;
			}
			return tptr;

		}
		//! 型情報でオブジェクトを作成
		static std::shared_ptr<Reflectable> CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept;
		/*! @brief 型情報でオブジェクトを作成して型変換
			@return 変換後のポインタを返す。存在しない場合はnullptrを返す。
		*/
		template<typename T>
		static std::shared_ptr<T> CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept {
			auto ptr = CreateObjectByStdTypeInfo(t_info);
			if (ptr == nullptr) { return nullptr; }
			auto tptr = std::dynamic_pointer_cast<T>(ptr);
			if (tptr == nullptr) {
				PE_LOG_ERROR("指定型に変換できませんでした。(type_indo:", t_info.name(), ",TargetType:", typeid(T).name(), ")");
				return nullptr;
			}
			return tptr;

		}
		//! 型からObjectTypeIDを取得する
		template<typename T>
		static std::string GetObjectTypeIDByType() {
			return GetObjectTypeIDByStdTypeInfo(typeid(T));
		}
		//! std::type_infoからObjectTypeIDを取得する
		static std::string GetObjectTypeIDByStdTypeInfo(const std::type_info& tinfo);
		//! ObjectTypeIDから型情報を取得する
		static const std::type_info& GetStdTypeInfoByObjectTypeID(const std::string& id);
	private:
		Reflection()noexcept;
		class Impl_;
		static Impl_& impl_();
		static void RegisterObject_(const std::type_info& tinfo ,const std::string& id, std::unique_ptr<private_::ClassInfo>&& class_info);

		//Reflectableに公開
		static const private_::ClassInfo* GetClassInfo_Reflectable(const std::type_info& ti)noexcept;
	};

	namespace private_ {
		//C::Reflectionを持っているか
		template<class C, typename T = void>
		struct HasReflectionDataRegisterer : public std::false_type {};
		template<class C> 
		struct HasReflectionDataRegisterer < C, decltype(&C::ReflectionDataRegisterer, std::declval<void>()) > : public std::true_type {};
		template<class C>
		constexpr bool HasReflection_v = typename HasReflectionDataRegisterer<C>::value;

		//Cが抽象クラスでない場合
		template<typename C, typename T = void>
		class ClassRegisterTrigger {
		public:
			//U::Reflectionを持っていない場合
			template<typename U, typename V = void>
			struct CallSwicher {
				CallSwicher(const char* object_id, const std::function<std::shared_ptr<Reflectable>()>& creator) {
					auto ci = std::make_unique<ClassInfo>();
					ci->is_abstract = false;
					ci->creator = creator;
					Reflection::RegisterObject<C>(object_id, std::move(ci));
				}
				CallSwicher(const char* object_id) {
					auto ci = std::make_unique<ClassInfo>();
					ci->is_abstract = false;
					ci->creator = [] {return std::make_shared<C>(); };
					Reflection::RegisterObject<C>(object_id, std::move(ci));
				}
			};
			//U::Reflectionを持っている場合
			template<typename U>
			struct CallSwicher<U, decltype(std::enable_if_t<HasReflection_v<U>>(), std::declval<void>())> {
				CallSwicher(const char* object_id, const std::function<std::shared_ptr<Reflectable>()>& creator) {
					auto ci = std::make_unique<ClassInfo>();
					ci->is_abstract = false;
					ci->creator = creator;
					ClassRegisterer<C> cregr(*ci);
					C::ReflectionDataRegisterer(cregr);
					Reflection::RegisterObject<C>(object_id, std::move(ci));
				}
				CallSwicher(const char* object_id) {
					auto ci = std::make_unique<ClassInfo>();
					ci->is_abstract = false;
					ci->creator = [] {return std::make_shared<C>(); };
					ClassRegisterer<C> cregr(*ci);
					C::ReflectionDataRegisterer(cregr);
					Reflection::RegisterObject<C>(object_id, std::move(ci));
				}
			};

			ClassRegisterTrigger(const char* object_id, const std::function<std::shared_ptr<Reflectable>()>& creator) { CallSwicher<C> cs(object_id, creator); }
			ClassRegisterTrigger(const char* object_id) { CallSwicher<C> cs(object_id); }
		};
		//Cが抽象クラスの場合
		template<typename C>
		class ClassRegisterTrigger<C, decltype(std::enable_if_t<std::is_abstract_v<C>>(), std::declval<void>())> {
		public:
			//U::Reflectionを持っていない場合
			template<typename U, typename V = void>
			struct CallSwicher {
				CallSwicher(const char* object_id) {
					auto ci = std::make_unique<ClassInfo>();
					ci->is_abstract = true;
					Reflection::RegisterObject<C>(object_id, std::move(ci));
				}
			};
			//U::Reflectionを持っている場合
			template<typename U>
			struct CallSwicher<U, decltype(std::enable_if_t<HasReflection_v<U>>(), std::declval<void>())> {
				CallSwicher(const char* object_id) {
					auto ci = std::make_unique<ClassInfo>();
					ci->is_abstract = true;
					ClassRegisterer<C> cregr(*ci);
					C::Reflection(cregr);
					Reflection::RegisterObject<C>(object_id, std::move(ci));
				}
			};
			ClassRegisterTrigger(const char* object_id) { CallSwicher<C> cs(object_id); }
		};
	}
}

namespace planeta {
	namespace private_ {
		//メンバクラス内静的変数がクラス単位で生成されることを利用して、複数回呼ばれても最初の一回しか登録処理を行わないようにする。
		template<typename T>
		struct ReflectableClassRegisterHelper {
			template<typename... Params>
			ReflectableClassRegisterHelper(Params... params) {
				static planeta::private_::ClassRegisterTrigger<T> class_register_trigger{ params... };
			}
		};
	}
}

/*! @def
	Objectをシステムに登録する(登録する型は、公開型エイリアスSuperが定義されていること)
	@param type 型
*/
#define PE_REFLECTABLE_CLASS(type)\
namespace { planeta::private_::ReflectableClassRegisterHelper<type> pe_reflectable_class_register_helper_##type##_ = {#type}; }

/*! @def
Objectをクリエータを指定してシステムに登録する(登録する型は、公開型エイリアスSuperが定義されていること)
@param type 型
@param creator クリエータ(std::shared_ptr<Object>()の関数型)
*/
#define PE_REFLECTABLE_CLASS_WITH_CREATOR(type, creator)\
namespace { planeta::private_::ReflectableClassRegisterHelper<type> pe_reflectable_class_register_helper_##type##_ = {#type, creator}; }
