#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

//http://stackoverflow.com/questions/32907385/cant-compile-boost-spirit-example4-cpp より
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
#include "boost/variant.hpp"

#include "ResourceBase.h"
#include "SystemLog.h"

namespace planeta {
	/*JSON型の不一致エラー*/
	class JSONTypeError final: public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	//全般的に、コピームーブの挙動の検証が必要。
	class JSONValue;
	class JSONObject final{
	public:
		//暗黙的変換を許可
		JSONObject(std::unordered_map<std::string, std::shared_ptr<JSONValue>>&& obj);
		std::shared_ptr<const JSONValue> At(const std::string& key)const noexcept;
		std::shared_ptr<const JSONValue> AtWithException(const std::string& key)const;
		auto begin()const { return obj_.begin(); }
		auto end()const { return obj_.end(); }
	private:
		std::unordered_map<std::string, std::shared_ptr<JSONValue>> obj_;
	};
	class JSONArray final{
	public:
		//暗黙的変換を許可
		JSONArray(std::vector<std::shared_ptr<JSONValue>>&& ary);
		std::shared_ptr<const JSONValue> At(size_t idx)const noexcept;
		std::shared_ptr<const JSONValue> AtWithException(size_t idx)const;
		size_t size()const;
		auto begin()const { return array_.begin(); }
		auto end()const { return array_.end(); }
	private:
		std::vector<std::shared_ptr<JSONValue>> array_;
	};
	class JSONNull final{};
	class JSONValue final{
		template<typename T>
		using sp = std::shared_ptr<T>;
	public:
		using JsonVariantType = boost::variant<sp<JSONNull>, sp<double>, sp<std::string>, sp<bool>, sp<JSONObject>, sp<JSONArray>>;
		//JSONValue();
		JSONValue(const JSONValue& obj);
		JSONValue(JSONValue&& obj);
		//各要素からの暗黙的変換を許可
		JSONValue(JsonVariantType&& var);
		~JSONValue();
		JSONValue& operator=(const JSONValue& obj);
		JSONValue& operator=(JSONValue&& obj);
		
		/*型を指定して値を取得する。(算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれか)*/
		template<class T>
		std::shared_ptr<const T> Get()const noexcept;
		/*型を指定して値を取得する。失敗した場合は例外を投げる。(算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれか)*/
		template<class T>
		std::shared_ptr<const T> GetWithException()const;
		/*型とデフォルト値を指定して値を取得する。(算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれか)*/
		template<typename T>
		std::shared_ptr<const T> GetWithDefault(T&& default_value)const noexcept;
		bool is_null()const;
	private:
		JsonVariantType var_;
		/*jSONValueの値を様々な型で取得するためのヘルパークラス*/
		//JSON組み込み型以外の数値型の場合
		template<class T>
		struct JSONValueGetter final{
			static std::shared_ptr<const T> GetWithException(const JSONValue::JsonVariantType& var) {
				//型が不正でないかチェック(ほかのJSON型は別のオーバーロードうや特殊化で処理されるはず)
				static_assert(
					std::conditional_t<std::is_same<T, JSONNull>::value, std::true_type,
					std::conditional_t<std::is_same<T, double>::value, std::true_type,
					std::conditional_t<std::is_same<T, std::string>::value, std::true_type,
					std::conditional_t<std::is_same<T, bool>::value, std::true_type,
					std::conditional_t<std::is_same<T, JSONObject>::value, std::true_type,
					std::conditional_t<std::is_same<T, JSONArray>::value, std::true_type,
					std::false_type>>>>>>::value==false,
					"T must not be double, JSONNull, std::string, bool, JSONObject, JSONArray type."
					);
				//double以外の数値型か確認
				static_assert(std::is_arithmetic<T>::value, "T must be a arithmetic type.");
				auto v = JSONValueGetter<double>::GetWithException(var);
				return std::make_shared<T>(static_cast<T>(*v));
			}
		};
		//JSONでの組み込み型の場合
#define JSON_VALUE_GETTER_FOR_JSONTYPE(ptype)\
		template<>\
		struct JSONValueGetter<ptype> final{\
			static std::shared_ptr<const ptype> GetWithException(const JSONValue::JsonVariantType& var) {\
				auto v = boost::get<std::shared_ptr<ptype>>(&var);\
				if (v) {\
					return *v;\
				} else {\
					throw JSONTypeError(util::ConvertAndConnectToString("型を変換できませんでした。ターゲットは\"",#ptype,"\"、ソースは\"", var.type().name(), "\"です。"));\
				}\
			}\
		};

JSON_VALUE_GETTER_FOR_JSONTYPE(bool);
JSON_VALUE_GETTER_FOR_JSONTYPE(double);
JSON_VALUE_GETTER_FOR_JSONTYPE(JSONNull);
JSON_VALUE_GETTER_FOR_JSONTYPE(std::string);
JSON_VALUE_GETTER_FOR_JSONTYPE(JSONObject);
JSON_VALUE_GETTER_FOR_JSONTYPE(JSONArray);

#undef  GET_WITH_EXCEPTION
		//std::vectorの場合
		template<class T, class Allocator>
		struct JSONValueGetter<std::vector<T, Allocator>> final{
			static std::shared_ptr<const std::vector<T, Allocator>> GetWithException(const JSONValue::JsonVariantType& var) {
				std::vector<T, Allocator> out;
				if (var.type() == typeid(std::shared_ptr<JSONArray>)) {
					auto ary = boost::get<std::shared_ptr<JSONArray>>(var);
					int idx = 0;
					for (auto&& elem : *ary) {
						try {
							out.push_back(*elem->GetWithException<T>());
						} catch (std::bad_cast& e) {
							throw JSONTypeError(util::ConvertAndConnectToString("At[Index:", idx, "],",e.what()));
						}
						++idx;
					}
					return std::make_shared<const std::vector<T, Allocator>>(std::move(out));
				} else {
					throw JSONTypeError("型をstd::vectorに変換できませんでした。JSONArray型である必要があります。");
				}
			}
		};
		//std::unordered_mapの場合
		template<class T, class Hasher, class KeyEQ, class Allocator>
		struct JSONValueGetter < std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>> final{
			static std::shared_ptr<const std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>>
				GetWithException(const JSONValue::JsonVariantType& var) {
				std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator> out;
				if (var.type() == typeid(std::shared_ptr<JSONObject>)) {
					auto obj = boost::get<std::shared_ptr<JSONObject>>(var);
					for (auto&& elem : *obj) {
						try {
							out.emplace(elem.first, *elem.second->GetWithException<T>());
						} catch (std::bad_cast& e) {
							throw JSONTypeError(util::ConvertAndConnectToString("At[Key:", elem.first, "],",e.what()));
						}
					}
					return std::make_shared<const std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>>(std::move(out));
				} else {
					throw JSONTypeError("型をstd::unordered_mapに変換できませんでした。JSONObject型である必要があります。");
				}
			}
		};
	};

	template<class T>
	std::shared_ptr<const T> JSONValue::Get()const noexcept {
		try {
			return GetWithException<T>();
		} catch (std::bad_cast& e) {
			PE_LOG_ERROR(e.what());
			return nullptr;
		}
	}

	template<class T>
	std::shared_ptr<const T> JSONValue::GetWithException()const {
		return JSONValueGetter<T>::GetWithException(var_);
	}

	template<class T>
	std::shared_ptr<const T> JSONValue::GetWithDefault(T&& default_value)const noexcept {
		try {
			return GetWithException<T>();
		} catch (std::bad_cast&) {
			return std::make_shared<const T>(std::move(default_value));
		}
	}
	
	/*JSONリソース*/
	class RJson final : public core::ResourceBase {
	public:
		RJson();
		~RJson();
		const JSONValue& GetRoot()const;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		bool _Create(const std::shared_ptr<const File>& file) override;
		void _Dispose() override;
	};
}