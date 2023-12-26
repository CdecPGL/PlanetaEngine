#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

//http://stackoverflow.com/questions/32907385/cant-compile-boost-spirit-example4-cpp ���
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
#include "boost/variant.hpp"

#include "LogUtility.hpp"

namespace plnt {
	//! JSON�^�̕s��v�G���[��O�N���X
	class JSONTypeError final : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	class JSONValue;
	/*! @brief Object�^��JSONValue�������N���X
	*/
	class JSONObject final {
	public:
		//! std::unordered_map����JSONObject���\�z����R���X�g���N�^�B�ÖٓI�ϊ�������
		JSONObject(std::unordered_map<std::string, std::shared_ptr<JSONValue>> &&obj);
		//! �w�肵���L�[�ɑΉ�����JSONValue���擾����
		std::shared_ptr<const JSONValue> At(const std::string &key) const noexcept;
		/*! @brief �w�肵���L�[�ɑΉ�����JSONValue���擾����B���݂��Ȃ��ꍇ�͗�O�𓊂���B
		@exception �L�[�����݂��Ȃ������ꍇ�Astd::out_of_rang�𓊂���
		*/
		std::shared_ptr<const JSONValue> AtWithException(const std::string &key) const;
		//! �擪�������C�e���[�^���擾����
		auto begin() const { return obj_.begin(); }
		//! �����������C�e���[�^���擾����
		auto end() const { return obj_.end(); }

	private:
		std::unordered_map<std::string, std::shared_ptr<JSONValue>> obj_;
	};

	/*! @brief Array�^��JSONValue�������N���X
	*/
	class JSONArray final {
	public:
		//! std::vector����JSONArray���\�z����R���X�g���N�^�B�ÖٓI�ϊ�������
		JSONArray(std::vector<std::shared_ptr<JSONValue>> &&ary);
		//! �w�肵���C���f�b�N�X�ɑΉ�����JSONValue���擾����
		std::shared_ptr<const JSONValue> At(size_t idx) const noexcept;
		/*! @brief �w�肵���C���f�b�N�X�ɑΉ�����JSONValue���擾����B���݂��Ȃ��ꍇ�͗�O�𓊂���B
		@exception �L�[�����݂��Ȃ������ꍇ�Astd::out_of_rang�𓊂���
		*/
		std::shared_ptr<const JSONValue> AtWithException(size_t idx) const;
		//! �z��̃T�C�Y���擾����
		size_t size() const;
		//! �擪�������C�e���[�^���擾����
		auto begin() const { return array_.begin(); }
		//! �����������C�e���[�^���擾����
		auto end() const { return array_.end(); }

	private:
		std::vector<std::shared_ptr<JSONValue>> array_;
	};

	/*! @brief null�^��JSONValue�������N���X
	*/
	class JSONNull final { };

	/*! @brief JSONValue�������N���X
	*/
	class JSONValue final {
		template <typename T>
		using sp = std::shared_ptr<T>;

	public:
		//! JSONValue��ێ����邽�߂�Variant�^
		using JsonVariantType = boost::variant<sp<JSONNull>, sp<double>, sp<std::string>, sp<bool>, sp<JSONObject>, sp<
			                                       JSONArray>>;
		//! �R�s�[�R���X�g���N�^
		JSONValue(const JSONValue &obj);
		//! ���[�u�R���X�g���N�^
		JSONValue(JSONValue &&obj);
		//! Variant����\�z����R���X�g���N�^�B�ÖٓI�ϊ���������
		JSONValue(JsonVariantType &&var);
		//! �f�X�g���N�^
		~JSONValue();
		//! ������Z�q
		JSONValue &operator=(const JSONValue &obj);
		//! ���[�u���Z�q
		JSONValue &operator=(JSONValue &&obj);
		/*! @brief �^���w�肵�Ēl���擾����B

		�e���v���[�g�p�����[�^�́A�Z�p�^,std::string,bool,JSONObject,JSONArray�A�܂��͂�����v�f�Ƃ���std::vector�Astd::unordered_map�̂����ꂩ�ł���K�v������B
		*/
		template <class T>
		std::shared_ptr<const T> Get() const noexcept;
		/*! @brief �^���w�肵�Ēl���擾����B���s�����ꍇ�͗�O�𓊂���B

		�e���v���[�g�p�����[�^�́A�Z�p�^,std::string,bool,JSONObject,JSONArray�A�܂��͂�����v�f�Ƃ���std::vector�Astd::unordered_map�̂����ꂩ�ł���K�v������B

		@exception �^�̕ϊ��Ɏ��s�����ꍇ�AJSONTypeError�𓊂���
		*/
		template <class T>
		std::shared_ptr<const T> GetWithException() const;
		/*! @brief �^�ƃf�t�H���g�l���w�肵�Ēl���擾����B

		�e���v���[�g�p�����[�^�́A�Z�p�^,std::string,bool,JSONObject,JSONArray�A�܂��͂�����v�f�Ƃ���std::vector�Astd::unordered_map�̂����ꂩ�ł���K�v������B
		*/
		template <typename T>
		std::shared_ptr<const T> GetWithDefault(T &&default_value) const noexcept;
		//! null���ǂ���
		bool is_null() const;
		/*! @brief �w�肵��Json�^�ł��邩

		�e���v���[�g�p�����[�^�́Adouble,std::string,bool,JSONObject,JSONArray,JSONNull�̂����ꂩ�ł���K�v������B
		*/
		template <typename T>
		bool is_json_type() const {
			decltype(auto) ptr = boost::get<T>(&var_);
			return ptr != nullptr;
		}

	private:
		JsonVariantType var_;
		/*jSONValue�̒l��l�X�Ȍ^�Ŏ擾���邽�߂̃w���p�[�N���X*/
		//JSON�g�ݍ��݌^�ȊO�̐��l�^�̏ꍇ
		template <class T>
		struct JSONValueGetter final {
			static std::shared_ptr<const T> GetWithException(const JSONValue::JsonVariantType &var) {
				//�^���s���łȂ����`�F�b�N(�ق���JSON�^�͕ʂ̃I�[�o�[���[�h������ꉻ�ŏ��������͂�)
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
				} else { throw JSONTypeError("�^��std::vector�ɕϊ��ł��܂���ł����BJSONArray�^�ł���K�v������܂��B"); }
			}
		};

		//std::unordered_map�̏ꍇ
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
				} else { throw JSONTypeError("�^��std::unordered_map�ɕϊ��ł��܂���ł����BJSONObject�^�ł���K�v������܂��B"); }
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
	/*! @brief JSON�t�@�C���������N���X
	@todo �S�ʓI�ɃR�s�[���[�u�̋����̌��؂��K�v�B
	*/
	class JsonFile final {
	public:
		//! ���̃R���X�g���N�^
		JsonFile();
		//! �f�X�g���N�^
		~JsonFile();
		//! Json�̃��[�g�l���擾����
		const JSONValue &GetRoot() const;
		//! �t�@�C������ǂݍ���
		bool Load(const File &file);
		//! �t�@�C�����J����Ă��邩�ǂ���
		bool is_opened() const;

	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
}
