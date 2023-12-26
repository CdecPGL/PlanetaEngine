#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

//http://stackoverflow.com/questions/32907385/cant-compile-boost-spirit-example4-cpp より
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
#include "boost/variant.hpp"

#include "LogUtility.hpp"

namespace plnt {
	//! JSON型の不一致エラー例外クラス
	class JSONTypeError final : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	class JSONValue;
	/*! @brief Object型のJSONValueを扱うクラス
	*/
	class JSONObject final {
	public:
		//! std::unordered_mapからJSONObjectを構築するコンストラクタ。暗黙的変換を許す
		JSONObject(std::unordered_map<std::string, std::shared_ptr<JSONValue>> &&obj);
		//! 指定したキーに対応するJSONValueを取得する
		std::shared_ptr<const JSONValue> At(const std::string &key) const noexcept;
		/*! @brief 指定したキーに対応するJSONValueを取得する。存在しない場合は例外を投げる。
		@exception キーが存在しなかった場合、std::out_of_rangを投げる
		*/
		std::shared_ptr<const JSONValue> AtWithException(const std::string &key) const;
		//! 先頭を示すイテレータを取得する
		auto begin() const { return obj_.begin(); }
		//! 末尾を示すイテレータを取得する
		auto end() const { return obj_.end(); }

	private:
		std::unordered_map<std::string, std::shared_ptr<JSONValue>> obj_;
	};

	/*! @brief Array型のJSONValueを扱うクラス
	*/
	class JSONArray final {
	public:
		//! std::vectorからJSONArrayを構築するコンストラクタ。暗黙的変換を許す
		JSONArray(std::vector<std::shared_ptr<JSONValue>> &&ary);
		//! 指定したインデックスに対応するJSONValueを取得する
		std::shared_ptr<const JSONValue> At(size_t idx) const noexcept;
		/*! @brief 指定したインデックスに対応するJSONValueを取得する。存在しない場合は例外を投げる。
		@exception キーが存在しなかった場合、std::out_of_rangを投げる
		*/
		std::shared_ptr<const JSONValue> AtWithException(size_t idx) const;
		//! 配列のサイズを取得する
		size_t size() const;
		//! 先頭を示すイテレータを取得する
		auto begin() const { return array_.begin(); }
		//! 末尾を示すイテレータを取得する
		auto end() const { return array_.end(); }

	private:
		std::vector<std::shared_ptr<JSONValue>> array_;
	};

	/*! @brief null型のJSONValueを扱うクラス
	*/
	class JSONNull final { };

	/*! @brief JSONValueを扱うクラス
	*/
	class JSONValue final {
		template <typename T>
		using sp = std::shared_ptr<T>;

	public:
		//! JSONValueを保持するためのVariant型
		using JsonVariantType = boost::variant<sp<JSONNull>, sp<double>, sp<std::string>, sp<bool>, sp<JSONObject>, sp<
			                                       JSONArray>>;
		//! コピーコンストラクタ
		JSONValue(const JSONValue &obj);
		//! ムーブコンストラクタ
		JSONValue(JSONValue &&obj);
		//! Variantから構築するコンストラクタ。暗黙的変換を許可する
		JSONValue(JsonVariantType &&var);
		//! デストラクタ
		~JSONValue();
		//! 代入演算子
		JSONValue &operator=(const JSONValue &obj);
		//! ムーブ演算子
		JSONValue &operator=(JSONValue &&obj);
		/*! @brief 型を指定して値を取得する。

		テンプレートパラメータは、算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれかである必要がある。
		*/
		template <class T>
		std::shared_ptr<const T> Get() const noexcept;
		/*! @brief 型を指定して値を取得する。失敗した場合は例外を投げる。

		テンプレートパラメータは、算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれかである必要がある。

		@exception 型の変換に失敗した場合、JSONTypeErrorを投げる
		*/
		template <class T>
		std::shared_ptr<const T> GetWithException() const;
		/*! @brief 型とデフォルト値を指定して値を取得する。

		テンプレートパラメータは、算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれかである必要がある。
		*/
		template <typename T>
		std::shared_ptr<const T> GetWithDefault(T &&default_value) const noexcept;
		//! nullかどうか
		bool is_null() const;
		/*! @brief 指定したJson型であるか

		テンプレートパラメータは、double,std::string,bool,JSONObject,JSONArray,JSONNullのいずれかである必要がある。
		*/
		template <typename T>
		bool is_json_type() const {
			decltype(auto) ptr = boost::get<T>(&var_);
			return ptr != nullptr;
		}

	private:
		JsonVariantType var_;
		/*jSONValueの値を様々な型で取得するためのヘルパークラス*/
		//JSON組み込み型以外の数値型の場合
		template <class T>
		struct JSONValueGetter final {
			static std::shared_ptr<const T> GetWithException(const JSONValue::JsonVariantType &var) {
				//型が不正でないかチェック(ほかのJSON型は別のオーバーロードうや特殊化で処理されるはず)
				static_assert(
					std::conditional_t<std::is_same<T, JSONNull>::value, std::true_type,
					                   std::conditional_t<std::is_same<T, double>::value, std::true_type,
					                                      std::conditional_t<
						                                      std::is_same<T, std::string>::value, std::true_type,
						                                      std::conditional_t<
							                                      std::is_same<T, bool>::value, std::true_type,
							                                      std::conditional_t<
								                                      std::is_same<T, JSONObject>::value, std::true_type
								                                      ,
								                                      std::conditional_t<
									                                      std::is_same<T, JSONArray>::value,
									                                      std::true_type,
									                                      std::false_type>>>>>>::value == false,
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
		template <class T, class Allocator>
		struct JSONValueGetter<std::vector<T, Allocator>> final {
			static std::shared_ptr<const std::vector<T, Allocator>> GetWithException(
				const JSONValue::JsonVariantType &var) {
				std::vector<T, Allocator> out;
				if (var.type() == typeid(std::shared_ptr<JSONArray>)) {
					auto ary = boost::get<std::shared_ptr<JSONArray>>(var);
					int idx = 0;
					for (auto &&elem : *ary) {
						try { out.push_back(*elem->GetWithException<T>()); } catch (std::bad_cast &e) {
							throw JSONTypeError(util::ConvertAndConnectToString("At[Index:", idx, "],", e.what()));
						}
						++idx;
					}
					return std::make_shared<const std::vector<T, Allocator>>(std::move(out));
				} else { throw JSONTypeError("型をstd::vectorに変換できませんでした。JSONArray型である必要があります。"); }
			}
		};

		//std::unordered_mapの場合
		template <class T, class Hasher, class KeyEQ, class Allocator>
		struct JSONValueGetter<std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>> final {
			static std::shared_ptr<const std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>>
			GetWithException(const JSONValue::JsonVariantType &var) {
				std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator> out;
				if (var.type() == typeid(std::shared_ptr<JSONObject>)) {
					auto obj = boost::get<std::shared_ptr<JSONObject>>(var);
					for (auto &&elem : *obj) {
						try { out.emplace(elem.first, *elem.second->GetWithException<T>()); } catch (std::bad_cast &e) {
							throw JSONTypeError(util::ConvertAndConnectToString("At[Key:", elem.first, "],", e.what()));
						}
					}
					return std::make_shared<const std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>>(
						std::move(out));
				} else { throw JSONTypeError("型をstd::unordered_mapに変換できませんでした。JSONObject型である必要があります。"); }
			}
		};
	};

	template <class T>
	std::shared_ptr<const T> JSONValue::Get() const noexcept {
		try { return GetWithException<T>(); } catch (std::bad_cast &e) {
			PE_LOG_ERROR(e.what());
			return nullptr;
		}
	}

	template <class T>
	std::shared_ptr<const T> JSONValue::GetWithException() const { return JSONValueGetter<T>::GetWithException(var_); }

	template <class T>
	std::shared_ptr<const T> JSONValue::GetWithDefault(T &&default_value) const noexcept {
		try { return GetWithException<T>(); } catch (std::bad_cast &) {
			return std::make_shared<const T>(std::move(default_value));
		}
	}

	class File;
	/*! @brief JSONファイルを扱うクラス
	@todo 全般的にコピームーブの挙動の検証が必要。
	*/
	class JsonFile final {
	public:
		//! 基底のコンストラクタ
		JsonFile();
		//! デストラクタ
		~JsonFile();
		//! Jsonのルート値を取得する
		const JSONValue &GetRoot() const;
		//! ファイルから読み込む
		bool Load(const File &file);
		//! ファイルが開かれているかどうか
		bool is_opened() const;

	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
}
