#include "picojson.h"

#include "JSONResource.h"
#include "File.h"
#include "SystemLog.h"

namespace planeta {
	namespace resources {
		namespace {
			JSONValue ConvertPicojsonValueToJSONValue(const picojson::value& pj_value) {
				if (pj_value.is<double>()) {
					return pj_value.get<double>();
				} else if (pj_value.is<std::string>()) {
					return pj_value.get<std::string>();
				} else if (pj_value.is<bool>()) {
					return pj_value.get<bool>();
				} else if (pj_value.is<picojson::object>()) {
					decltype(auto) pj_obj = pj_value.get<picojson::object>();
					JSONObject out;
					for (auto&& v : pj_obj) {
						auto key = v.first;
						auto value = std::move(ConvertPicojsonValueToJSONValue(v.second));
						out.emplace(key, std::move(value));
					}
					return out;
				} else if (pj_value.is<picojson::array>()) {
					decltype(auto) pj_ary = pj_value.get<picojson::array>();
					JSONArray out;
					for (auto&& v : pj_ary) {
						out.push_back(std::move(ConvertPicojsonValueToJSONValue(pj_value)));
					}
					return out;
				}else if (pj_value.is<picojson::null>()) {
					return nullptr;
				} else {
					assert(false);
					return nullptr;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//JSONValue
		//////////////////////////////////////////////////////////////////////////
		JSONValue::JSONValue() :JSONValue(nullptr) {}
		JSONValue::JSONValue(const JSONValue& obj) = default;
		JSONValue::JSONValue(JSONValue&& obj) :var_(std::move(obj.var_)) {}
		JSONValue::JSONValue(boost::variant<JSONNull, double, std::string, bool, JSONObject, JSONArray>&& var) : var_(std::move(var)) {}
		JSONValue::~JSONValue() = default;
		JSONValue& JSONValue::operator= (const JSONValue& obj) = default;
		JSONValue& JSONValue::operator=(JSONValue&& obj) = default;

		template<typename T>
		boost::optional<const T&> JSONValue::Get()const {
			const T* v = boost::get<T>(&var_);
			if (v) {
				return *v;
			} else {
				return boost::none;
			}
		}

		template boost::optional<const int&> JSONValue::Get<int>()const;
		template boost::optional<const double&> JSONValue::Get<double>()const;
		template boost::optional<const std::string&> JSONValue::Get<std::string>()const;
		template boost::optional<const bool&> JSONValue::Get<bool>()const;
		template boost::optional<const JSONObject&> JSONValue::Get<JSONObject>()const;
		template boost::optional<const JSONArray&> JSONValue::Get<JSONArray>()const;

		//////////////////////////////////////////////////////////////////////////
		//JSONResource::Impl_
		//////////////////////////////////////////////////////////////////////////
		class JSONResource::Impl_ {
		public:
			JSONValue value;
		};
		//////////////////////////////////////////////////////////////////////////
		//JSONResource
		//////////////////////////////////////////////////////////////////////////
		JSONResource::JSONResource() :impl_(std::make_unique<Impl_>()) {}
		JSONResource::~JSONResource() = default;

		bool JSONResource::_Create(const std::shared_ptr<const File>& file) {
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

		void JSONResource::_Dispose() {
		}

		const JSONValue& JSONResource::GetRoot() const {
			return impl_->value;
		}

	}
}
