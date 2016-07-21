#pragma  once

#include <unordered_map>
#include <typeindex>

#include "boost/multi_index_container.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/hashed_index.hpp"

#include "Reflection.h"
#include "ReflectionExceptions.h"
#include "StringUtility.h"

namespace planeta {
	using namespace core;
	namespace bmi = boost::multi_index;
	namespace {
		struct TypeData {
			TypeData(const std::type_info& ti, const std::string& pid, const std::function<std::shared_ptr<core::Object>()>& crtr)
				:std_type_info(ti), std_type_index(ti), id(pid), creator(crtr) {}
			const std::type_info& std_type_info;
			std::type_index std_type_index;
			std::string id;
			std::function<std::shared_ptr<core::Object>()> creator;
		};
	}
	namespace tag{
		struct StdTypeInfo {};
		struct ID {};
	}
	//////////////////////////////////////////////////////////////////////////
	//Reflection::Impl_
	//////////////////////////////////////////////////////////////////////////
	class Reflection::Impl_ {
	public:
		using TypeDataMapType = bmi::multi_index_container<TypeData, bmi::indexed_by<
			bmi::hashed_unique<bmi::tag<tag::StdTypeInfo>, bmi::member<TypeData, std::type_index, &TypeData::std_type_index>>,
			bmi::hashed_unique<bmi::tag<tag::ID>, bmi::member<TypeData, std::string, &TypeData::id>>
			>>;
		TypeDataMapType type_data_map;
	};

	//////////////////////////////////////////////////////////////////////////
	//Reflection
	//////////////////////////////////////////////////////////////////////////

	Reflection::Reflection() = default;

	Reflection::Impl_& Reflection::impl_() {
		static Impl_ impl;
		return impl;
	}

	std::shared_ptr<Object> Reflection::CreateObjectByObjectTypeID(const std::string& id)noexcept {
		decltype(auto) id_map = impl_().type_data_map.get<tag::ID>();
		auto it = id_map.find(id);
		if (it == id_map.end()) {
			PE_LOG_ERROR("存在しないObject(ID:", id, ")が指定されました。");
			return nullptr;
		} else {
			return (it->creator)();
		}
	}

	std::shared_ptr<Object> Reflection::CreateObjectByStdTypeInfo(const std::type_info& t_info)noexcept {
		decltype(auto) std_ti_map = impl_().type_data_map.get<tag::StdTypeInfo>();
		auto it = std_ti_map.find(t_info);
		if (it == std_ti_map.end()) {
			PE_LOG_ERROR("存在しないObject(Type:", t_info.name(), ")が指定されました。");
			return nullptr;
		} else {
			return (it->creator)();
		}
	}

	std::string Reflection::GetObjectTypeIDByStdTypeInfo(const std::type_info& tinfo) {
		decltype(auto) std_ti_map = impl_().type_data_map.get<tag::StdTypeInfo>();
		auto it = std_ti_map.find(tinfo);
		if (it == std_ti_map.end()) {
			throw reflection_error(util::ConvertAndConnectToString("登録されていない型\"", tinfo.name(), "\"が指定されました。"));
		}
		return it->id;
	}

	const std::type_info& Reflection::GetStdTypeInfoByObjectTypeID(const std::string& id) {
		decltype(auto) id_map = impl_().type_data_map.get<tag::ID>();
		auto it = id_map.find(id);
		if (it == id_map.end()) {
			throw reflection_error(util::ConvertAndConnectToString("登録されていない型ID\"", id, "\"が指定されました。"));
		}
		return it->std_type_info;
	}

	void Reflection::RegisterObject_(const std::type_info& tinfo, const std::string& id, const std::function<std::shared_ptr<core::Object>()>& creator) {
		if (!impl_().type_data_map.insert({ tinfo,id,creator }).second) {
			throw reflection_error(util::ConvertAndConnectToString("型\"", tinfo.name(), "\"が重複登録されました。(ID:\"", id, "\")"));
		}
	}
}