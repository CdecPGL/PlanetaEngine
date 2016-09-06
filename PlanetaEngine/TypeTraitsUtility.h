#pragma once

#include "boost/core/enable_if.hpp"

#include <tuple>

namespace planeta {
	namespace util {
		/*!�^�v���ɗv�f��ǉ�*/
		template<typename...>
		struct TupleAdd {};
		template<typename... TArgs, typename... NArgs>
		struct TupleAdd<std::tuple<TArgs...>, NArgs...> {
			using type = std::tuple<TArgs..., NArgs...>;
		};
		/*! @brief �X�[�p�[�N���X���^�v���Ŏ擾����
			
			Super���w�肳��Ă��Ȃ��N���X���AL�ɒB����܂ł̃X�[�p�[�N���X���^�v���ŕԂ����B
		*/
		template<typename T, typename L = void, typename U = void>
		struct GetSuperClassAsTuple {
			using type = typename std::tuple<T>;
		};
		template<typename T, typename L>
		struct GetSuperClassAsTuple<T, L, decltype(std::declval<typename T::Super>(),void())> {
			using type = std::conditional_t<std::is_same_v<T, L>, std::tuple<T>, typename TupleAdd<GetSuperClassAsTuple<typename T::Super>, T>::type>;
		};
	}
}
