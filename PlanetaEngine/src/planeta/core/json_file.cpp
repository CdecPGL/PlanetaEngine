#include "json_file.hpp"

#include "picojson/picojson.h"

#include "file.hpp"
#include "log_utility.hpp"
#include "character_code.hpp"

namespace plnt {
	namespace {
		std::shared_ptr<json_value> convert_picojson_value_to_json_value(const picojson::value &pj_value) {
			if (pj_value.is<double>()) {
				return std::make_shared<json_value>(std::make_shared<double>(pj_value.get<double>()));
			}
			if (pj_value.is<std::string>()) {
				//UTF8からシステム文字コードに変換
				return std::make_shared<json_value>(
					std::make_shared<std::string>(util::convert_utf8_to_system_code(pj_value.get<std::string>())));
			}
			if (pj_value.is<bool>()) {
				return std::make_shared<json_value>(std::make_shared<bool>(pj_value.get<bool>()));
			}
			if (pj_value.is<picojson::object>()) {
				decltype(auto) pj_obj = pj_value.get<picojson::object>();
				std::unordered_map<std::string, std::shared_ptr<json_value>> out;
				for (const auto &[k, pv] : pj_obj) {
					auto key = k;
					auto value = convert_picojson_value_to_json_value(pv);
					out.emplace(key, std::move(value));
				}
				return std::make_shared<json_value>(std::make_shared<json_object>(std::move(out)));
			}
			if (pj_value.is<picojson::array>()) {
				decltype(auto) pj_ary = pj_value.get<picojson::array>();
				std::vector<std::shared_ptr<json_value>> out;
				for (auto &&v : pj_ary) { out.push_back(convert_picojson_value_to_json_value(v)); }
				return std::make_shared<json_value>(std::make_shared<json_array>(std::move(out)));
			}
			if (pj_value.is<picojson::null>()) { return std::make_shared<json_value>(std::make_shared<json_null>()); }
			assert(false);
			return nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//JSONObject
	//////////////////////////////////////////////////////////////////////////
	json_object::json_object(
		std::unordered_map<std::string, std::shared_ptr<json_value>> &&obj) : obj_(std::move(obj)) { }

	std::shared_ptr<const json_value> json_object::at(const std::string &key) const noexcept {
		try { return at_with_exception(key); } catch (std::out_of_range &e) {
			PE_LOG_ERROR(e.what());
			return nullptr;
		}
	}

	std::shared_ptr<const json_value> json_object::at_with_exception(const std::string &key) const {
		const auto it = obj_.find(key);
		if (it == obj_.end()) {
			throw std::out_of_range(util::ConvertAndConnectToString("Key\"", key, "\"のJSONValueは存在しません。"));
		}
		return it->second;
	}

	//////////////////////////////////////////////////////////////////////////
	//JSONArray
	//////////////////////////////////////////////////////////////////////////
	json_array::json_array(std::vector<std::shared_ptr<json_value>> &&ary) : array_(std::move(ary)) { }

	std::shared_ptr<const json_value> json_array::at(const size_t idx) const noexcept {
		try { return at_with_exception(idx); } catch (std::out_of_range &e) {
			PE_LOG_ERROR(e.what());
			return nullptr;
		}
	}

	std::shared_ptr<const json_value> json_array::at_with_exception(size_t idx) const {
		if (idx >= array_.size()) {
			throw std::out_of_range(
				util::ConvertAndConnectToString("Index\"", idx, "\"は範囲[0,", array_.size(), "]外です。"));
		}
		return array_[idx];
	}

	size_t json_array::size() const { return array_.size(); }

	//////////////////////////////////////////////////////////////////////////
	//JSONValue
	//////////////////////////////////////////////////////////////////////////
	//JSONValue::JSONValue() :JSONValue(std::make_shared<JSONNull>()) {}
	json_value::json_value(const json_value &obj) = default;

	json_value::json_value(json_value &&obj) noexcept : var_(std::move(obj.var_)) { }

	json_value::json_value(json_variant_type &&var) : var_(std::move(var)) { }

	json_value::~json_value() = default;
	json_value &json_value::operator=(const json_value &obj) = default;
	json_value &json_value::operator=(json_value &&obj) noexcept = default;

	bool json_value::is_null() const { return var_.which() == 0; }

	//////////////////////////////////////////////////////////////////////////
	//JSONResource::Impl_
	//////////////////////////////////////////////////////////////////////////
	class json_file::impl {
	public:
		std::shared_ptr<json_value> value;
	};

	//////////////////////////////////////////////////////////////////////////
	//JsonFile
	//////////////////////////////////////////////////////////////////////////
	json_file::json_file() : impl_(std::make_unique<impl>()) { }

	json_file::~json_file() = default;

	bool json_file::load(const file &file) const {
		using namespace picojson;
		std::string err;
		value v;
		parse(v, file.top_pointer(), file.top_pointer() + file.size(), &err);
		if (!err.empty()) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。(", err, ")");
			return false;
		}
		impl_->value = convert_picojson_value_to_json_value(v);
		return true;
	}

	const json_value &json_file::get_root() const { return *impl_->value; }

	bool json_file::is_opened() const { return impl_->value != nullptr; }
}
