#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

//http://stackoverflow.com/questions/32907385/cant-compile-boost-spirit-example4-cpp ���
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
#include "boost/variant.hpp"

#include "ResourceBase.h"
#include "SystemLog.h"

namespace planeta {
	/*JSON�^�̕s��v�G���[*/
	class JSONTypeError final: public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	//�S�ʓI�ɁA�R�s�[���[�u�̋����̌��؂��K�v�B
	class JSONValue;
	class JSONObject final{
	public:
		//�ÖٓI�ϊ�������
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
		//�ÖٓI�ϊ�������
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
		//�e�v�f����̈ÖٓI�ϊ�������
		JSONValue(JsonVariantType&& var);
		~JSONValue();
		JSONValue& operator=(const JSONValue& obj);
		JSONValue& operator=(JSONValue&& obj);
		
		/*�^���w�肵�Ēl���擾����B(�Z�p�^,std::string,bool,JSONObject,JSONArray�A�܂��͂�����v�f�Ƃ���std::vector�Astd::unordered_map�̂����ꂩ)*/
		template<class T>
		std::shared_ptr<const T> Get()const noexcept;
		/*�^���w�肵�Ēl���擾����B���s�����ꍇ�͗�O�𓊂���B(�Z�p�^,std::string,bool,JSONObject,JSONArray�A�܂��͂�����v�f�Ƃ���std::vector�Astd::unordered_map�̂����ꂩ)*/
		template<class T>
		std::shared_ptr<const T> GetWithException()const;
		/*�^�ƃf�t�H���g�l���w�肵�Ēl���擾����B(�Z�p�^,std::string,bool,JSONObject,JSONArray�A�܂��͂�����v�f�Ƃ���std::vector�Astd::unordered_map�̂����ꂩ)*/
		template<typename T>
		std::shared_ptr<const T> GetWithDefault(T&& default_value)const noexcept;
		bool is_null()const;
	private:
		JsonVariantType var_;
		/*jSONValue�̒l��l�X�Ȍ^�Ŏ擾���邽�߂̃w���p�[�N���X*/
		//JSON�g�ݍ��݌^�ȊO�̐��l�^�̏ꍇ
		template<class T>
		struct JSONValueGetter final{
			static std::shared_ptr<const T> GetWithException(const JSONValue::JsonVariantType& var) {
				//�^���s���łȂ����`�F�b�N(�ق���JSON�^�͕ʂ̃I�[�o�[���[�h������ꉻ�ŏ��������͂�)
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
				//double�ȊO�̐��l�^���m�F
				static_assert(std::is_arithmetic<T>::value, "T must be a arithmetic type.");
				auto v = JSONValueGetter<double>::GetWithException(var);
				return std::make_shared<T>(static_cast<T>(*v));
			}
		};
		//JSON�ł̑g�ݍ��݌^�̏ꍇ
#define JSON_VALUE_GETTER_FOR_JSONTYPE(ptype)\
		template<>\
		struct JSONValueGetter<ptype> final{\
			static std::shared_ptr<const ptype> GetWithException(const JSONValue::JsonVariantType& var) {\
				auto v = boost::get<std::shared_ptr<ptype>>(&var);\
				if (v) {\
					return *v;\
				} else {\
					throw JSONTypeError(util::ConvertAndConnectToString("�^��ϊ��ł��܂���ł����B�^�[�Q�b�g��\"",#ptype,"\"�A�\�[�X��\"", var.type().name(), "\"�ł��B"));\
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
		//std::vector�̏ꍇ
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
					throw JSONTypeError("�^��std::vector�ɕϊ��ł��܂���ł����BJSONArray�^�ł���K�v������܂��B");
				}
			}
		};
		//std::unordered_map�̏ꍇ
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
					throw JSONTypeError("�^��std::unordered_map�ɕϊ��ł��܂���ł����BJSONObject�^�ł���K�v������܂��B");
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
	
	/*JSON���\�[�X*/
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