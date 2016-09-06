#pragma once

#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>

#include <map>
#include <unordered_map>

#include <type_traits>
#include <iostream>

#include "boost/property_tree/ptree.hpp"
#include "boost/core/enable_if.hpp"

#define PE_PTREE_CONVERT_TO_ARRAY_TYPE(array_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(array_type<T, Rest...>& dst, const boost::property_tree::ptree& src) {\
	for (auto&& pp : src) {\
		if (pp.first.empty() == false) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("�z��^��Ptree�L�[�͋�ł���K�v������܂��B(�ǂݎ��ꂽ�L�[:",pp.first,")"));\
		}\
		T dat{};\
		ReflectivePtreeConverter(dat, pp.second);\
		dst.push_back(std::move(dat));\
	}\
}
#define PE_PTREE_CONVERT_TO_SET_TYPE(set_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(set_type<T, Rest...>& dst, const boost::property_tree::ptree& src) {\
	for (auto&& pp : src) {\
		if (pp.first.empty() == false) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("�z��^��Ptree�L�[�͋�ł���K�v������܂��B(�ǂݎ��ꂽ�L�[:",pp.first,")"));\
		}\
		T dat{};\
		ReflectivePtreeConverter(dat, pp.second);\
		dst.insert(std::move(dat));\
	}\
}
#define PE_PTREE_CONVERT_TO_MAP_TYPE(map_type)\
template<typename T, typename... Rest>\
void ReflectivePtreeConverter(map_type<std::string, T, Rest...>& dst, const boost::property_tree::ptree& src) {\
	for (auto&& pp : src) {\
		if (pp.first.empty() == true) {\
			throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("�}�b�v�^��Ptree�L�[�͋�ł����Ă͂����܂���B"));\
		}\
		T dat{};\
		ReflectivePtreeConverter(dat, pp.second);\
		dst.emplace(pp.first, std::move(dat));\
	}\
}

namespace planeta {
	namespace private_ {
		template<typename T, typename... Rest>
		void ReflectivePtreeConverterError() {
			throw reflection_error(util::ConvertAndConnectToString("Ptree����̕ϊ��ɑΉ����Ă��Ȃ��^\"", typeid(T).name(), "\"���w�肳��܂����B"));
		}
	}
	namespace util {
		/*! �ϊ��s�\�Ȍ^��Ptree�ϊ��֐�*/
		template<typename... Ts>
		void ReflectivePtreeConverter(Ts...) {
			private_::ReflectivePtreeConverterError<Ts...>();
		}
		/*! @brief ptree���璼�ڕϊ��\�Ȍ^�ւ�Ptree�ϊ��֐�
			@note BoostLibrary1.61.0�ł́Aget_value����istream�ɂ��^�̕ϊ����s���Ă���B����𗘗p���āAget_value�ɑΉ����Ă��Ȃ��^�ɑ΂��ẮAstd::cin�ł��R���p�C���G���[�ɂȂ邱�Ƃ𗘗p����SFINE��p���I�[�o�[���[�h�ΏۊO�ɂ���Bget_value�̓��������Ɉˑ����Ă��邽�߁A����ɕύX���������ꍇ�͏C������K�v������B
		*/
		template<typename T>
		auto ReflectivePtreeConverter(T& dst, const boost::property_tree::ptree& src) -> decltype(std::cin >> dst, std::declval<typename boost::disable_if<std::is_base_of<Reflectable, T>, void>::type>()) {
			try {
				dst = src.get_value<T>();
			} catch (boost::property_tree::ptree_bad_data& e) {
				throw reflection_error(util::ConvertAndConnectToString("Ptree����^\"", typeid(T).name(), "\"�ւ̕ϊ��Ɏ��s���܂����B(", e.what(), ")"));
			}
		}
		/*! Object���p�������^�ւ�Ptree�ϊ��֐�*/
		template<typename T>
		auto ReflectivePtreeConverter(T& dst, const boost::property_tree::ptree& src) -> typename boost::enable_if<std::is_base_of<Reflectable, T>, void>::type {
			dst.LoadFromPtree(src);
		}
		/*! �z��^�ւ�Ptree�ϊ��֐�*/
		PE_PTREE_CONVERT_TO_ARRAY_TYPE(std::vector);
		PE_PTREE_CONVERT_TO_ARRAY_TYPE(std::list);
		PE_PTREE_CONVERT_TO_ARRAY_TYPE(std::deque);
		/*! �Z�b�g�^�ւ�Ptree�ϊ��֐�*/
		PE_PTREE_CONVERT_TO_SET_TYPE(std::set);
		PE_PTREE_CONVERT_TO_SET_TYPE(std::unordered_set);
		/*! �}�b�v�^�ւ�Ptree�ϊ��֐�*/
		PE_PTREE_CONVERT_TO_MAP_TYPE(std::map);
		PE_PTREE_CONVERT_TO_MAP_TYPE(std::unordered_map);
	}
}

#undef PE_PTREE_CONVERT_TO_ARRAY_TYPE
#undef PE_PTREE_CONVERT_TO_SET_TYPE
#undef PE_PTREE_CONVERT_TO_MAP_TYPE


namespace planeta {
	class Reflectable;
	namespace util {
		/*! Reflectable���p�����Ă��Ȃ��^�̃R�s�[�n���h��*/
		template<typename T>
		auto ReflectiveCopyHandler(T& dst, const T& src) -> typename boost::disable_if<std::is_base_of<Reflectable, T>, void>::type {
			dst = src;
		}
		/*! Reflectable���p�����Ă���^�̃R�s�[�n���h��*/
		template<typename T>
		auto ReflectiveCopyHandler(T& dst, const T& src) -> typename boost::enable_if<std::is_base_of<Reflectable, T>, void>::type {
			dst.CopyFrom(src);
		}
	}
}