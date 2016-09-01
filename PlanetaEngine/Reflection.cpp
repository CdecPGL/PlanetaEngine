#pragma  once

#include <unordered_map>

#include "boost/multi_index_container.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/hashed_index.hpp"

#include "Reflection.h"
#include "ReflectionExceptions.h"
#include "StringUtility.h"

namespace planeta {
	namespace {
		constexpr char* REFLECTION_ROOT_OBJECT_TYPE_ID("ReflectionRoot");
	}
	using namespace private_;
	namespace bmi = boost::multi_index;
	namespace tag{
		struct StdTypeInfo {};
		struct ObjectTypeID {};
	}
	//////////////////////////////////////////////////////////////////////////
	//Reflection::Impl_
	//////////////////////////////////////////////////////////////////////////
	class Reflection::Impl_ {
	public:
		using TypeDataMapType = bmi::multi_index_container<std::unique_ptr<ClassInfo>, bmi::indexed_by<
			bmi::hashed_unique<bmi::tag<tag::StdTypeInfo>, bmi::member<ClassInfo, TypeInfoWrapper, &ClassInfo::this_t_info>, TypeInfoWrapper::hash>,
			bmi::hashed_unique<bmi::tag<tag::ObjectTypeID>, bmi::member<ClassInfo, std::string, &ClassInfo::object_type_id>>
			>>;
		TypeDataMapType type_data_map;
		bool is_initialized_ = false;
		ClassInfo* reflection_root_class_info = nullptr;
		//コンストラクタ
		Impl_()noexcept {
			//リフレクションルートを登録
			auto rrci = std::make_unique<ClassInfo>();
			rrci->this_t_info = typeid(Reflectable);
			rrci->object_type_id = REFLECTION_ROOT_OBJECT_TYPE_ID;
			rrci->super_t_info = typeid(void);
			reflection_root_class_info = rrci.get();
			type_data_map.insert(std::move(rrci));
		}
		//登録されたクラスデータから親子関係データを構築し、各種データを設定する
		void ProcessReflectionData() {
			decltype(auto) std_ti_map = impl_().type_data_map.get<tag::StdTypeInfo>();
			//親子のポインタをセット
			for (auto&& this_class_info_u : std_ti_map) {
				ClassInfo* this_class_info = this_class_info_u.get();
				//リフレクションルートでなかったら親クラスに関する処理を行う
				if (this_class_info->object_type_id != REFLECTION_ROOT_OBJECT_TYPE_ID) {
					//親クラスのデータセット
					const std::type_info& sti = this_class_info->super_t_info.get_type_info();
					auto it = std_ti_map.find(sti);
					if (it == std_ti_map.end()) {
						throw reflection_error(util::ConvertAndConnectToString("ObjectTypeID\"", this_class_info->object_type_id, "\"の親クラス(std::type_info\"", sti.name(), "\")が登録されていません。"));
					}
					ClassInfo* super_class_info = (*it).get();
					this_class_info->super_class_info = super_class_info;
					//親クラスに自分のクラスを登録
					super_class_info->child_t_info.emplace(this_class_info->this_t_info);
					super_class_info->child_class_info.emplace(this_class_info);
				}
			}
			//整合性確認と関数変数設定
			std::function<void(ClassInfo&)> create_full_info = [&create_full_info](ClassInfo& ci) {
				//親クラスが存在したら、自クラスに存在しない親クラスの変数を登録する
				if (ci.super_class_info != nullptr) {
					auto& super_class_info = *ci.super_class_info;
					//整合性確認
					auto it = ci.super_class_info->child_t_info.find(ci.this_t_info);
					if (it == ci.super_class_info->child_t_info.end()) {
						//親クラスの子として自分が登録されていない
						throw reflection_error(util::ConvertAndConnectToString("登録情報の整合性がありません。\"", ci.object_type_id, "\"の親クラス\"", super_class_info.object_type_id, "\"に自クラスが子クラスとして設定されていません。"));
					}
					//親クラスの変数関数追加
					for (auto&& var_prop : super_class_info.public_variable_prpperty_info) {
						auto var_id = var_prop.first;
						//親クラスにある変数のうち、自分のクラスにないものだったら追加
						if (ci.public_variable_prpperty_info.find(var_id) == ci.public_variable_prpperty_info.end()) {
							ci.public_variable_prpperty_info.emplace(var_id, var_prop.second);
						}
					}
					//親クラスのコピーハンドラ継承
					ci.copy_handler_list.reserve(ci.copy_handler_list.size() + super_class_info.copy_handler_list.size());
					ci.copy_handler_list.insert(ci.copy_handler_list.begin(), super_class_info.copy_handler_list.begin(), super_class_info.copy_handler_list.end());
				}
				//子クラスに再帰的に適用
				for (auto&& cci : ci.child_class_info) {
					//整合性確認
					if (cci->super_t_info != ci.this_t_info) {
						//子クラスの親が自分でない
						throw reflection_error(util::ConvertAndConnectToString("登録情報の整合性がありません。\"", ci.object_type_id, "\"の子クラス\"", cci->object_type_id, "\"に自クラスが親クラスとして設定されていません。\"", cci->super_class_info->object_type_id, "\"が設定されています。"));
					}
					create_full_info(const_cast<ClassInfo&>(*cci));
				}
			};
			create_full_info(*reflection_root_class_info);
			
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//Reflection
	//////////////////////////////////////////////////////////////////////////

	Reflection::Reflection()noexcept = default;

	Reflection::Impl_& Reflection::impl_() {
		static Impl_ impl;
		return impl;
	}


	std::shared_ptr<Reflectable> Reflection::CreateObjectByObjectTypeID(const std::string& id)noexcept {
		decltype(auto) id_map = impl_().type_data_map.get<tag::ObjectTypeID>();
		auto it = id_map.find(id);
		if (it == id_map.end()) {
			PE_LOG_ERROR("存在しないObject(ID:", id, ")が指定されました。");
			return nullptr;
		} else {
			if ((*it)->is_abstract) {
				PE_LOG_ERROR("ObjectTypeID\"", id, "\"に関連付けられたオブジェクトは抽象クラスのため、インスタンスを作成することはできません。");
				return nullptr;
			}
			return ((*it)->creator)();
		}
	}

	std::shared_ptr<Reflectable> Reflection::CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept {
		decltype(auto) std_ti_map = impl_().type_data_map.get<tag::StdTypeInfo>();
		auto it = std_ti_map.find(t_info);
		if (it == std_ti_map.end()) {
			PE_LOG_ERROR("存在しないObject(Type:", t_info.name(), ")が指定されました。");
			return nullptr;
		} else {
			if ((*it)->is_abstract) {
				PE_LOG_ERROR("std::type_info\"", t_info.name(), "\"に関連付けられたオブジェクトは抽象クラスのため、インスタンスを作成することはできません。");
				return nullptr;
			}
			return ((*it)->creator)();
		}
	}

	std::string Reflection::GetObjectTypeIDByStdTypeInfo(const std::type_info& tinfo) {
		decltype(auto) std_ti_map = impl_().type_data_map.get<tag::StdTypeInfo>();
		auto it = std_ti_map.find(tinfo);
		if (it == std_ti_map.end()) {
			throw reflection_error(util::ConvertAndConnectToString("登録されていない型\"", tinfo.name(), "\"が指定されました。"));
		}
		return (*it)->object_type_id;
	}

	const std::type_info& Reflection::GetStdTypeInfoByObjectTypeID(const std::string& id) {
		decltype(auto) id_map = impl_().type_data_map.get<tag::ObjectTypeID>();
		auto it = id_map.find(id);
		if (it == id_map.end()) {
			throw reflection_error(util::ConvertAndConnectToString("登録されていない型ID\"", id, "\"が指定されました。"));
		}
		return (*it)->this_t_info.get_type_info();
	}

	void Reflection::RegisterObject_(const std::type_info& tinfo, const std::string& object_type_id, std::unique_ptr<ClassInfo>&& class_info) {
		if (impl_().is_initialized_) {
			throw reflection_error("オブジェクトの登録は初期化前に行われなければなりません。");
		}
		class_info->object_type_id = object_type_id;
		if (!impl_().type_data_map.insert(std::move(class_info)).second) {
			throw reflection_error(util::ConvertAndConnectToString("型\"", tinfo.name(), "\"が重複登録されました。(ID:\"", object_type_id, "\")"));
		}
	}

	const ClassInfo* Reflection::GetClassInfo_Reflectable(const std::type_info& t_info)noexcept {
		decltype(auto) std_ti_map = impl_().type_data_map.get<tag::StdTypeInfo>();
		auto it = std_ti_map.find(t_info);
		if (it == std_ti_map.end()) {
			PE_LOG_ERROR("存在しないObject(Type:", t_info.name(), ")が指定されました。登録されていないクラスの可能性があります。");
			return nullptr;
		}
		return (*it).get();
	}

	bool Reflection::Initialize() {
		impl_().ProcessReflectionData();
		impl_().is_initialized_ = true;
		return true;
	}

}