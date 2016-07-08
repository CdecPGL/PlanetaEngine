#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "boost/optional.hpp"
//http://stackoverflow.com/questions/32907385/cant-compile-boost-spirit-example4-cpp ‚æ‚è
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
#include "boost/variant.hpp"

#include "ResourceBase.h"

namespace planeta {
	namespace resources {
		class JSONValue;
		using JSONObject = std::unordered_map<std::string, JSONValue>;
		using JSONArray = std::vector<JSONValue>;
		using JSONNull = std::nullptr_t;
		class JSONValue {
		public:
			JSONValue();
			JSONValue(const JSONValue& obj);
			JSONValue(JSONValue&& obj);
			//Še—v‘f‚©‚ç‚ÌˆÃ–Ù“I•ÏŠ·‚ð‹–‰Â
			JSONValue(boost::variant<JSONNull,double,std::string,bool,JSONObject,JSONArray>&& var);
			~JSONValue();
			JSONValue& operator=(const JSONValue& obj);
			JSONValue& operator=(JSONValue&& obj);
			template<typename T>
			boost::optional<const T&> Get()const;
		private:
			boost::variant<JSONNull, double, std::string, bool, JSONObject, JSONArray> var_;
		};

		class JSONResource final : public core::ResourceBase {
		public:
			JSONResource();
			~JSONResource();
			const JSONValue& GetRoot()const;
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
			bool _Create(const std::shared_ptr<const File>& file) override;
			void _Dispose() override;
		};
	}
}