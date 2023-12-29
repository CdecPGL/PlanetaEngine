#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

//http://stackoverflow.com/questions/32907385/cant-compile-boost-spirit-example4-cpp より
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
#include "boost/variant.hpp"

#include "log_utility.hpp"

namespace plnt {
	//! JSON型の不一致エラー例外クラス
	class json_type_error final : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	class json_value;
	/*! @brief Object型のjson_valueを扱うクラス
	*/
	class json_object final {
	public:
		//! std::unordered_mapからJSONObjectを構築するコンストラクタ。暗黙的変換を許す
		explicit json_object(std::unordered_map<std::string, std::shared_ptr<json_value>> &&obj);
		//! 指定したキーに対応するjson_valueを取得する
		[[nodiscard]] std::shared_ptr<const json_value> at(const std::string &key) const noexcept;
		/*! @brief 指定したキーに対応するjson_valueを取得する。存在しない場合は例外を投げる。
		@exception キーが存在しなかった場合、std::out_of_rangを投げる
		*/
		[[nodiscard]] std::shared_ptr<const json_value> at_with_exception(const std::string &key) const;
		//! 先頭を示すイテレータを取得する
		[[nodiscard]] auto begin() const { return obj_.begin(); }
		//! 末尾を示すイテレータを取得する
		[[nodiscard]] auto end() const { return obj_.end(); }

	private:
		std::unordered_map<std::string, std::shared_ptr<json_value>> obj_;
	};

	/*! @brief Array型のjson_valueを扱うクラス
	*/
	class json_array final {
	public:
		//! std::vectorからJSONArrayを構築するコンストラクタ。暗黙的変換を許す
		explicit json_array(std::vector<std::shared_ptr<json_value>> &&ary);
		//! 指定したインデックスに対応するjson_valueを取得する
		[[nodiscard]] std::shared_ptr<const json_value> at(size_t idx) const noexcept;
		/*! @brief 指定したインデックスに対応するjson_valueを取得する。存在しない場合は例外を投げる。
		@exception キーが存在しなかった場合、std::out_of_rangを投げる
		*/
		[[nodiscard]] std::shared_ptr<const json_value> at_with_exception(size_t idx) const;
		//! 配列のサイズを取得する
		[[nodiscard]] size_t size() const;
		//! 先頭を示すイテレータを取得する
		[[nodiscard]] auto begin() const { return array_.begin(); }
		//! 末尾を示すイテレータを取得する
		[[nodiscard]] auto end() const { return array_.end(); }

	private:
		std::vector<std::shared_ptr<json_value>> array_;
	};

	/*! @brief null型のjson_valueを扱うクラス
	*/
	class json_null final { };

	/*! @brief json_valueを扱うクラス
	*/
	class json_value final {
		template <typename T>
		using sp = std::shared_ptr<T>;

	public:
		//! json_valueを保持するためのVariant型
		using json_variant_type = boost::variant<
			sp<json_null>, sp<double>, sp<std::string>, sp<bool>, sp<json_object>, sp<
				json_array>>;
		//! コピーコンストラクタ
		json_value(const json_value &obj);
		//! ムーブコンストラクタ
		json_value(json_value &&obj) noexcept;
		//! Variantから構築するコンストラクタ。暗黙的変換を許可する
		explicit json_value(json_variant_type &&var);
		//! デストラクタ
		~json_value();
		//! 代入演算子
		json_value &operator=(const json_value &obj);
		//! ムーブ演算子
		json_value &operator=(json_value &&obj) noexcept;
		/*! @brief 型を指定して値を取得する。

		テンプレートパラメータは、算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれかである必要がある。
		*/
		template <class T>
		[[nodiscard]] std::shared_ptr<const T> get() const noexcept;
		/*! @brief 型を指定して値を取得する。失敗した場合は例外を投げる。

		テンプレートパラメータは、算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれかである必要がある。

		@exception 型の変換に失敗した場合、json_type_errorを投げる
		*/
		template <class T>
		[[nodiscard]] std::shared_ptr<const T> get_with_exception() const;
		/*! @brief 型とデフォルト値を指定して値を取得する。

		テンプレートパラメータは、算術型,std::string,bool,JSONObject,JSONArray、またはそれらを要素とするstd::vector、std::unordered_mapのいずれかである必要がある。
		*/
		template <typename T>
		[[nodiscard]] std::shared_ptr<const T> get_with_default(T &&default_value) const noexcept;
		//! nullかどうか
		[[nodiscard]] bool is_null() const;
		/*! @brief 指定したJson型であるか

		テンプレートパラメータは、double,std::string,bool,JSONObject,JSONArray,JSONNullのいずれかである必要がある。
		*/
		template <typename T>
		[[nodiscard]] bool is_json_type() const {
			decltype(auto) ptr = boost::get<T>(&var_);
			return ptr != nullptr;
		}

	private:
		json_variant_type var_;
		/*jSONValueの値を様々な型で取得するためのヘルパークラス*/
		//JSON組み込み型以外の数値型の場合
		template <class T>
		struct json_value_getter final {
			[[nodiscard]] static std::shared_ptr<const T> get_with_exception(const json_variant_type &var) {
				//型が不正でないかチェック(ほかのJSON型は別のオーバーロードうや特殊化で処理されるはず)
				static_assert(
					std::conditional_t<std::is_same_v<T, json_null>, std::true_type,
					                   std::conditional_t<std::is_same_v<T, double>, std::true_type,
					                                      std::conditional_t<
						                                      std::is_same_v<T, std::string>, std::true_type,
						                                      std::conditional_t<
							                                      std::is_same_v<T, bool>, std::true_type,
							                                      std::conditional_t<
								                                      std::is_same_v<T, json_object>, std::true_type,
								                                      std::conditional_t<
									                                      std::is_same_v<T, json_array>,
									                                      std::true_type,
									                                      std::false_type>>>>>>::value == false,
					"T must not be double, JSONNull, std::string, bool, JSONObject, JSONArray type."
				);
				//double以外の数値型か確認
				static_assert(std::is_arithmetic_v<T>, "T must be a arithmetic type.");
				const auto v = json_value_getter<double>::get_with_exception(var);
				return std::make_shared<T>(static_cast<T>(*v));
			}
		};

		//JSONでの組み込み型の場合
		#define JSON_VALUE_GETTER_FOR_JSONTYPE(p_type)\
		template<>\
		struct json_value_getter<p_type> final{\
			[[nodiscard]] static std::shared_ptr<const p_type> get_with_exception(const json_value::json_variant_type& var) {\
				auto v = boost::get<std::shared_ptr<p_type>>(&var);\
				if (v) {\
					return *v;\
				} else {\
					throw json_type_error(util::ConvertAndConnectToString("型を変換できませんでした。ターゲットは\"",#p_type,"\"、ソースは\"", var.type().name(), "\"です。"));\
				}\
			}\
		}

		JSON_VALUE_GETTER_FOR_JSONTYPE(bool);

		JSON_VALUE_GETTER_FOR_JSONTYPE(double);

		JSON_VALUE_GETTER_FOR_JSONTYPE(json_null);

		JSON_VALUE_GETTER_FOR_JSONTYPE(std::string);

		JSON_VALUE_GETTER_FOR_JSONTYPE(json_object);

		JSON_VALUE_GETTER_FOR_JSONTYPE(json_array);

		#undef  GET_WITH_EXCEPTION
		//std::vectorの場合
		template <class T, class Allocator>
		struct json_value_getter<std::vector<T, Allocator>> final {
			[[nodiscard]] static std::shared_ptr<const std::vector<T, Allocator>> get_with_exception(
				const json_variant_type &var) {
				if (var.type() == typeid(std::shared_ptr<json_array>)) {
					std::vector<T, Allocator> out;
					const auto ary = boost::get<std::shared_ptr<json_array>>(var);
					int idx = 0;
					for (auto &&elem : *ary) {
						try { out.push_back(*elem->get_with_exception<T>()); } catch (std::bad_cast &e) {
							throw json_type_error(util::ConvertAndConnectToString("At[Index:", idx, "],", e.what()));
						}
						++idx;
					}
					return std::make_shared<const std::vector<T, Allocator>>(std::move(out));
				}
				throw json_type_error("型をstd::vectorに変換できませんでした。JSONArray型である必要があります。");
			}
		};

		//std::unordered_mapの場合
		template <class T, class Hasher, class KeyEQ, class Allocator>
		struct json_value_getter<std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>> final {
			[[nodiscard]] static std::shared_ptr<const std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>>
			get_with_exception(const json_variant_type &var) {
				if (var.type() == typeid(std::shared_ptr<json_object>)) {
					std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator> out;
					for (const auto obj = boost::get<std::shared_ptr<json_object>>(var); const auto &[key, value] : *
					     obj) {
						try { out.emplace(key, *value->get_with_exception<T>()); } catch (std::bad_cast &
							e) {
							throw json_type_error(
								util::ConvertAndConnectToString("At[Key:", key, "],", e.what()));
						}
					}
					return std::make_shared<const std::unordered_map<std::string, T, Hasher, KeyEQ, Allocator>>(
						std::move(out));
				}
				throw json_type_error("型をstd::unordered_mapに変換できませんでした。JSONObject型である必要があります。");
			}
		};
	};

	template <class T>
	[[nodiscard]] std::shared_ptr<const T> json_value::get() const noexcept {
		try { return get_with_exception<T>(); } catch (std::bad_cast &e) {
			PE_LOG_ERROR(e.what());
			return nullptr;
		}
	}

	template <class T>
	[[nodiscard]] std::shared_ptr<const T> json_value::get_with_exception() const {
		return json_value_getter<T>::get_with_exception(var_);
	}

	template <class T>
	[[nodiscard]] std::shared_ptr<const T> json_value::get_with_default(T &&default_value) const noexcept {
		try { return get_with_exception<T>(); } catch (std::bad_cast &) {
			return std::make_shared<const T>(std::move(default_value));
		}
	}

	class file;
	/*! @brief JSONファイルを扱うクラス
	@todo 全般的にコピームーブの挙動の検証が必要。
	*/
	class json_file final {
	public:
		//! 基底のコンストラクタ
		json_file();
		json_file(const json_file &) = delete;
		json_file(json_file &&) noexcept = delete;
		//! デストラクタ
		~json_file();
		json_file &operator=(const json_file &) = delete;
		json_file &operator=(json_file &&) noexcept = delete;

		//! Jsonのルート値を取得する
		[[nodiscard]] const json_value &get_root() const;
		//! ファイルから読み込む
		bool load(const file &file) const;
		//! ファイルが開かれているかどうか
		[[nodiscard]] bool is_opened() const;

	private:
		class impl;
		std::unique_ptr<impl> impl_;
	};
}
