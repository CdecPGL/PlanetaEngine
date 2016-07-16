#define PICOJSON_USE_INT64 //picojsonでint64型を有効にする
#include "picojson.h"

#include "RJson.h"
#include "File.h"
#include "SystemLog.h"
#include "CharacterCode.h"

namespace planeta {
	namespace {
		JSONValue ConvertPicojsonValueToJSONValue(const picojson::value& pj_value) {
			if (pj_value.is<double>()) {
				return pj_value.get<double>();
			} else if (pj_value.is<int64_t>()){
				return pj_value.get<int64_t>();
			} else if (pj_value.is<std::string>()) {
				//UTF8からシステム文字コードに変換
				return util::ConvertUTF8ToSystemCode(pj_value.get<std::string>());
			} else if (pj_value.is<bool>()) {
				return pj_value.get<bool>();
			} else if (pj_value.is<picojson::object>()) {
				decltype(auto) pj_obj = pj_value.get<picojson::object>();
				std::unordered_map<std::string, JSONValue> out;
				for (auto&& v : pj_obj) {
					auto key = v.first;
					auto value = std::move(ConvertPicojsonValueToJSONValue(v.second));
					out.emplace(key, std::move(value));
				}
				return std::move(out);
			} else if (pj_value.is<picojson::array>()) {
				decltype(auto) pj_ary = pj_value.get<picojson::array>();
				std::vector<JSONValue> out;
				for (auto&& v : pj_ary) {
					out.push_back(std::move(ConvertPicojsonValueToJSONValue(v)));
				}
				return std::move(out);
			} else if (pj_value.is<picojson::null>()) {
				return nullptr;
			} else {
				assert(false);
				return nullptr;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//JSONObject
	//////////////////////////////////////////////////////////////////////////
	JSONObject::JSONObject(std::unordered_map<std::string, JSONValue>&& obj) :obj_(std::move(obj)) {}
	boost::optional<const JSONValue&> JSONObject::At(const std::string& key)const {
		auto it = obj_.find(key);
		if (it == obj_.end()) {
			return boost::none;
		} else {
			return it->second;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//JSONArray
	//////////////////////////////////////////////////////////////////////////
	JSONArray::JSONArray(std::vector<JSONValue>&& ary) :array_(std::move(ary)) {}
	boost::optional<const JSONValue&> JSONArray::At(size_t idx)const {
		if (idx >= array_.size()) {
			return boost::none;
		} else {
			return array_[idx];
		}
	}
	size_t JSONArray::size()const { return array_.size(); }

	//////////////////////////////////////////////////////////////////////////
	//JSONValue
	//////////////////////////////////////////////////////////////////////////
	JSONValue::JSONValue() :JSONValue(nullptr) {}
	JSONValue::JSONValue(const JSONValue& obj) = default;
	JSONValue::JSONValue(JSONValue&& obj) :var_(std::move(obj.var_)) {}
	JSONValue::JSONValue(JsonVariantType&& var) : var_(std::move(var)) {}
	JSONValue::~JSONValue() = default;
	JSONValue& JSONValue::operator= (const JSONValue& obj) = default;
	JSONValue& JSONValue::operator=(JSONValue&& obj) = default;

	bool JSONValue::is_null()const {
		return var_.which() == 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//JSONResource::Impl_
	//////////////////////////////////////////////////////////////////////////
	class RJson::Impl_ {
	public:
		JSONValue value;
	};
	//////////////////////////////////////////////////////////////////////////
	//JSONResource
	//////////////////////////////////////////////////////////////////////////
	RJson::RJson() :impl_(std::make_unique<Impl_>()) {}
	RJson::~RJson() = default;

	bool RJson::_Create(const std::shared_ptr<const File>& file) {
		using namespace picojson;
		std::string err;
		value v;
		parse(v, file->GetTopPointer(), file->GetTopPointer() + file->GetSize(), &err);
		if (!err.empty()) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。(", err, ")");
			return false;
		}
		impl_->value = std::move(ConvertPicojsonValueToJSONValue(v));
		return true;
	}

	void RJson::_Dispose() {}

	const JSONValue& RJson::GetRoot() const {
		return impl_->value;
	}
}
