#pragma once

#include <array>
#include <initializer_list>
#include <cassert>
#include <type_traits>

#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/if.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"

#include "Vector2D.h"

namespace planeta_engine {
	namespace math {
//���W���x�����C���f�b�N�X����擾����
#define CGS_COORDINATELABEL_GETTER(idx)\
BOOST_PP_IF(idx,BOOST_PP_IF(BOOST_PP_SUB(idx,1),BOOST_PP_IF(BOOST_PP_SUB(idx, 2),w,z),y),x)
//�C���f�b�N�X������W�v���p�e�B(�Q�b�^�A�Z�b�^)�𐶐�����
#define CGS_GETTER_SETTER_DEFINER(idx)\
T CGS_COORDINATELABEL_GETTER(idx)()const{return ptr_[idx];};\
void CGS_COORDINATELABEL_GETTER(idx)(const T& v){ptr_[idx] = v;};
//���boost::pp�Ŏg���悤���b�p��������
#define CGS_GETTER_SETTER_DEFINER_WRAPPER(z,idx,data)\
CGS_GETTER_SETTER_DEFINER(idx)
//0-[count-1]�܂ł̍��W�v���p�e�B�𐶐�����B
#define CGS_GETTET_SETTER_DEFINER_IDX_0_TO_ARG(count)\
BOOST_PP_REPEAT(count,CGS_GETTER_SETTER_DEFINER_WRAPPER,0)
//0-[count-1]�܂ł̍��W�v���p�e�B�����C���^�[�t�F�C�X�N���X�𐶐�����
#define CGS_DEFINER(id,dimension)\
template<typename T>\
class id {\
public:\
id(T* ptr) :ptr_(ptr) {}\
CGS_GETTET_SETTER_DEFINER_IDX_0_TO_ARG(dimension)\
private:\
T* ptr_;\
};
		namespace cgs {
			//���W�v���p�e�B�N���X
			CGS_DEFINER(XGetterSetter, 1);
			CGS_DEFINER(XYGetterSetter, 2);
			CGS_DEFINER(XYZGetterSetter, 3);
			CGS_DEFINER(XYZWGetterSetter, 4);
			//�����ɂ����W�v���p�e�B�N���X��I������G�C���A�X�e���v���[�g
			template<int S, typename T>
			using CoordinateGetterSetterSelector = std::conditional_t<S == 1, XGetterSetter<T>, std::conditional_t<S == 2, XYGetterSetter<T>, std::conditional_t<S == 3, XYZGetterSetter<T>, XYZWGetterSetter<T>>>>;
		}
		/*�x�N�g���N���X<�v�f�̌^,�v�f��>*/
		template<typename T,int S>
		class Vector : public cgs::CoordinateGetterSetterSelector<S,T> {
			/*�T�C�Y��0���傫�����m�F*/
			static_assert(S > 0, "Invalid Vector size. Vector size must be bigger than 0");
			/*�^���Z�p�^���m�F*/
			static_assert(std::is_arithmetic<T>::value == true, "The element type must to be an arithmetic type.");
			using MyType = Vector<T, S>;
			using CoordinateGetterSetter = cgs::CoordinateGetterSetterSelector<S,T>;
		public:
			/*�f�t�H���g�R���X�g���N�^�B�v�f��S��0�ɂ���*/
			Vector() :CoordinateGetterSetter(elements_.data()) { elements_.fill(0); }
			/*�R�s�[�R���X�g���N�^*/
			Vector(const MyType& v) :CoordinateGetterSetter(elements_.data()),elements_(v.elements_) {}
			/*���[�u�R���X�g���N�^(std::array�ŗv�f�����̂Ŗ��Ӗ�)*/
//			Vector(MyType&& v) :elements_(std::move(v.elements_)) {}
			/*�ϒ��̒l���Ƃ�R���X�g���N�^(�ÖٓI�ϊ������ۂ���Ə��������X�g�ŏ������ł��Ȃ�)*/
			template<typename... ArgT>
			Vector(const T& first_arg, ArgT... args) : CoordinateGetterSetter(elements_.data()), elements_({ first_arg, args... }) {
				static_assert(S == (sizeof...(ArgT)+1), "The number of constructor argument must equal to Vector size.");
			}
			/*�T�C�Y��^�̈قȂ�x�N�g���ŏ�����(�ÖٓI�ϊ�������)*/
			template<typename T2,int S2>
			Vector(const Vector<T2, S2>& v): CoordinateGetterSetter(elements_.data()) {
				for (int i = 0; i < S; ++i) {
					(*this)[i] = i < S2 ? v[i] : 0;
				}
			}
			//������Z�q
			MyType& operator=(const MyType& v) { elements_ = v.elements_; return *this; }
			//���[�u���Z�q(std::array�ŗv�f�����̂Ŗ��Ӗ�)
//			MyType& operator=(MyType&& v) { elements_ = std::move(v.elements_); return *this; }
			/*�T�C�Y��^�̈قȂ�x�N�g������*/
			template<typename T2, int S2>
			Vector<T,S>& operator=(const Vector<T2, S2>& v) {
				for (int i = 0; i < S; ++i) {
					(*this)[i] = static_cast<T>(i < S2 ? v[i] : 0);
				}
				return *this;
			}
			//�������Z�q(�T�C�Y���قȂ�ꍇ�͂��Ԃ镔���݂̂Ŕ�r)
			template<typename T2,int S2>
			bool operator==(const Vector<T2, S2>& v)const {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) { if ((*this)[i] != v[i]) { return false; } }
				return true;
			}
			template<typename T2, int S2>
			bool operator!=(const Vector<T2, S2>& v)const {
				return !(*this == v);
			}
			//�v�f�ɃA�N�Z�X
			T& operator[](size_t idx) { return elements_[idx]; }
			const T& operator[](size_t idx)const { return elements_[idx]; }
			//�x�N�g���Ƃ̉��Z
			template<typename T2,int S2>
			const Vector<T,S>& operator+=(const Vector<T2, S2>& v) {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					(*this)[i] += v[i];
				}
				return *this;
			}
			template<typename T2, int S2>
			auto operator+(const Vector<T2, S2>& v)const {
				constexpr int MaxS = S < S2 ? S2 : S;
				Vector<decltype(std::declval<T>() + std::declval<T2>()), MaxS> ret;
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					ret[i] += v[i];
				}
				return std::move(ret);
			}
			//�x�N�g���Ƃ̌��Z
			template<typename T2, int S2>
			const MyType& operator-=(const Vector<T2, S2>& v) {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					(*this)[i] -= v[i];
				}
				return *this;
			}
			template<typename T2, int S2>
			auto operator-(const Vector<T2, S2>& v)const {
				constexpr int MaxS = S < S2 ? S2 : S;
				Vector<decltype(std::declval<T>() - std::declval<T2>()), MaxS> ret;
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					ret[i] -= v[i];
				}
				return std::move(ret);
			}
			//�X�J���[�Ƃ̏�Z
			template<typename T2>
			const Vector<T,S>& operator*=(const T2& v) {
				for (int i = 0; i < S; ++i) { (*this)[i] *= v; }
				return *this;
			}
			template<typename T2>
			auto operator*(const T2& v)const {
				Vector<decltype(std::declval<T>()*std::declval<T2>()), S> ret;
				for (int i = 0; i < S; ++i) { ret[i] *= v; }
				return std::move(ret);
			}
			//�X�J���[�Ƃ̏��Z
			template<typename T2>
			const MyType& operator/=(const T2& v) {
				assert(v != 0);
				for (int i = 0; i < S; ++i) { (*this)[i] /= v; }
				return *this;
			}
			template<typename T2>
			auto operator/(const T2& v)const {
				assert(v != 0);
				Vector<decltype(std::declval<T>()/std::declval<T2>()),S> ret;
				for (int i = 0; i < S; ++i) { ret[i] /= v; }
				return std::move(ret);
			}
			/*�P�����Z�q-*/
			MyType operator-()const {
				MyType ret = *this;
				for (int i = 0; i < S; ++i) { ret[i] = -ret[i]; }
				return std::move(ret);
			}

			/*�����̓��*/
			T length_square()const{
				T ret = 0;
				for (int i = 0; i < S; ++i) { ret += (*this)[i] * (*this)[i]; }
				return ret;
			}
			/*����*/
			auto length()const { return std::sqrt(length_square()); }
			/*�v�f�̃Z�b�g*/
			template<typename... ArgT>
			void Set(ArgT... args) {
				static_assert(S == (sizeof...(ArgT)), "The number of set function argument must equal to Vector size.");
				elements_ = { args... };
			}

			/*Vector2D�Ƃ̌݊��p*/
			//Vector2D����Vector�ւ̕ϊ�(�ÖٓI�ϊ�������)
			template<typename T2>
			Vector(const ::Vector2D<T2>& v2d) : Vector(Vector<T2, 2>(v2d.x, v2d.y)) {}
			//Vector����Vector2D�ւ̕ϊ�
			template<typename T2>
			operator ::Vector2D<T2>() {
				return ::Vector2D<T2>((*this)[0], S >= 2 ? (*this)[1] : 0);
			}

		private:
			std::array<T,S> elements_;
		};

		//�g�ݍ��݌^�Ƃ̉��Z�q
		template<typename SCALA_T, typename VECTOR_T, int VECTOR_S>
		auto operator*(const SCALA_T& s, const Vector<VECTOR_T, VECTOR_S>& v) {
			return v*s;
		}
	}
}