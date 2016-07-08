#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "boost/optional.hpp"
//http://stackoverflow.com/questions/32907385/cant-compile-boost-spirit-example4-cpp ���
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
#include "boost/variant.hpp"

#include "ResourceBase.h"

namespace planeta {
	namespace resources {
		//�S�ʓI�ɁA�R�s�[���[�u�̋����̌��؂��K�v�B
		class JSONValue;
		class JSONObject {
		public:
			//�ÖٓI�ϊ�������
			JSONObject(std::unordered_map<std::string, JSONValue>&& obj);
			boost::optional<const JSONValue&> At(const std::string& key)const;
			template<typename T>
			boost::optional<const T&> At(const std::string& key)const {
				decltype(auto) obj = At(key);
				if (obj) {
					return obj->Get<T>();
				} else {
					return boost::none;
				}
			}
		private:
			std::unordered_map<std::string, JSONValue> obj_;
		};
		class JSONArray {
		public:
			//�ÖٓI�ϊ�������
			JSONArray(std::vector<JSONValue>&& ary);
			boost::optional<const JSONValue&> At(size_t idx)const;
			template<typename T>
			boost::optional<const T&> At(size_t idx)const {
				decltype(auto) obj = At(idx);
				if (obj) {
					return obj->Get<T>();
				} else {
					return boost::none;
				}
			}
			size_t size()const;
		private:
			std::vector<JSONValue> array_;
		};
		using JSONNull = std::nullptr_t;
		class JSONValue {
		public:
			JSONValue();
			JSONValue(const JSONValue& obj);
			JSONValue(JSONValue&& obj);
			//�e�v�f����̈ÖٓI�ϊ�������
			JSONValue(boost::variant<JSONNull,double,std::string,bool,JSONObject,JSONArray>&& var);
			~JSONValue();
			JSONValue& operator=(const JSONValue& obj);
			JSONValue& operator=(JSONValue&& obj);
			/*�^���w�肵�Ēl���擾����B(double,std::string,bool,JSONObject,JSONArray�̂����ꂩ)*/
			template<typename T>
			boost::optional<const T&> Get()const {
				static_assert(
					std::conditional_t<std::is_same<T,JSONNull>::value,std::true_type,
					std::conditional_t<std::is_same<T, double>::value, std::true_type,
					std::conditional_t<std::is_same<T, std::string>::value, std::true_type,
					std::conditional_t<std::is_same<T, bool>::value, std::true_type,
					std::conditional_t<std::is_same<T, JSONObject>::value, std::true_type,
					std::conditional_t<std::is_same<T, JSONArray>::value, std::true_type,
					std::false_type>>>>>>::value,
					"T mest be JSONNull, double, std::string, bool, JSONObject or JSONArray."
					);
				const T* v = boost::get<T>(&var_);
				if (v) {
					return *v;
				} else {
					return boost::none;
				}
			}
			bool is_null()const;
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