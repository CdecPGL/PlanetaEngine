#include "JsonFile.hpp"

#include "picojson/picojson.h"

#include "file.hpp"
#include "LogUtility.hpp"
#include "character_code.hpp"

namespace plnt {
	namespace {
		std::shared_ptr<JSONValue> ConvertPicojsonValueToJSONValue(const picojson::value &pj_value) {
			if (pj_value.is<double>()) {
				return std::make_shared<JSONValue>(std::make_shared<double>(pj_value.get<double>()));
			} else if (pj_value.is<std::string>()) {
				//UTF8からシステム文字コードに変換
				return std::make_shared<JSONValue>(
					std::make_shared<std::string>(util::convert_utf8_to_system_code(pj_value.get<std::string>())));
			} else if (pj_value.is<bool>()) {
				return std::make_shared<JSONValue>(std::make_shared<bool>(pj_value.get<bool>()));
			} else if (pj_value.is<picojson::object>()) {
				decltype(auto) pj_obj = pj_value.get<picojson::object>();
				std::unordered_map<std::string, std::shared_ptr<JSONValue>> out;
				for (auto &&v : pj_obj) {
					auto key = v.first;
					auto value = std::move(ConvertPicojsonValueToJSONValue(v.second));
					out.emplace(key, std::move(value));
				}
				return std::make_shared<JSONValue>(std::make_shared<JSONObject>(std::move(out)));
			} else if (pj_value.is<picojson::array>()) {
				decltype(auto) pj_ary = pj_value.get<picojson::array>();
				std::vector<std::shared_ptr<JSONValue>> out;
				for (auto &&v : pj_ary) { out.push_back(std::move(ConvertPicojsonValueToJSONValue(v))); }
				return std::make_shared<JSONValue>(std::make_shared<JSONArray>(std::move(out)));
			} else if (pj_value.is<picojson::null>()) {
				return std::make_shared<JSONValue>(std::make_shared<JSONNull>());
			} else {
				assert(false);
				return nullptr;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//JSONObject
	//////////////////////////////////////////////////////////////////////////
	JSONObject::JSONObject(std::unordered_map<std::string, std::shared_ptr<JSONValue>> &&obj) : obj_(std::move(obj)) { }

	std::shared_ptr<const JSONValue> JSONObject::At(const std::string &key) const noexcept {
		try { return AtWithException(key); } catch (std::out_of_range &e) {
			PE_LOG_ERROR(e.what());
			return nullptr;
		}
	}

	std::shared_ptr<const JSONValue> JSONObject::AtWithException(const std::string &key) const {
		auto it = obj_.find(key);
		if (it == obj_.end()) {
			throw std::out_of_range(util::ConvertAndConnectToString("Key\"", key, "\"のJSONValueは存在しません。"));
		} else { return it->second; }
	}

	//////////////////////////////////////////////////////////////////////////
	//JSONArray
	//////////////////////////////////////////////////////////////////////////
	JSONArray::JSONArray(std::vector<std::shared_ptr<JSONValue>> &&ary) : array_(std::move(ary)) { }

	std::shared_ptr<const JSONValue> JSONArray::At(size_t idx) const noexcept {
		try { return AtWithException(idx); } catch (std::out_of_range &e) {
			PE_LOG_ERROR(e.what());
			return nullptr;
		}
	}

	std::shared_ptr<const JSONValue> JSONArray::AtWithException(size_t idx) const {
		if (idx >= array_.size()) {
			throw std::out_of_range(
				util::ConvertAndConnectToString("Index\"", idx, "\"は範囲[0,", array_.size(), "]外です。"));
		} else { return array_[idx]; }
	}

	size_t JSONArray::size() const { return array_.size(); }

	//////////////////////////////////////////////////////////////////////////
	//JSONValue
	//////////////////////////////////////////////////////////////////////////
	//JSONValue::JSONValue() :JSONValue(std::make_shared<JSONNull>()) {}
	JSONValue::JSONValue(const JSONValue &obj) = default;

	JSONValue::JSONValue(JSONValue &&obj) : var_(std::move(obj.var_)) { }

	JSONValue::JSONValue(JsonVariantType &&var) : var_(std::move(var)) { }

	JSONValue::~JSONValue() = default;
	JSONValue &JSONValue::operator=(const JSONValue &obj) = default;
	JSONValue &JSONValue::operator=(JSONValue &&obj) = default;

	bool JSONValue::is_null() const { return var_.which() == 0; }

	//////////////////////////////////////////////////////////////////////////
	//JSONResource::Impl_
	//////////////////////////////////////////////////////////////////////////
	class JsonFile::Impl_ {
	public:
		std::shared_ptr<JSONValue> value;
	};

	//////////////////////////////////////////////////////////////////////////
	//JsonFile
	//////////////////////////////////////////////////////////////////////////
	JsonFile::JsonFile() : impl_(std::make_unique<Impl_>()) { }

	JsonFile::~JsonFile() = default;

	bool JsonFile::Load(const file &file) {
		using namespace picojson;
		std::string err;
		value v;
		parse(v, file.top_pointer(), file.top_pointer() + file.size(), &err);
		if (!err.empty()) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。(", err, ")");
			return false;
		}
		impl_->value = std::move(ConvertPicojsonValueToJSONValue(v));
		return true;
	}

	const JSONValue &JsonFile::GetRoot() const { return *impl_->value; }

	bool JsonFile::is_opened() const { return impl_->value != nullptr; }
}
