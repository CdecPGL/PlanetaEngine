#pragma once

#include <array>
#include <initializer_list>
#include <cassert>
#include <type_traits>

#include "VectorElementIdentifer.h"

#include "Vector2D.h"

namespace planeta_engine {
	namespace math {
		/*�x�N�g���N���X<�v�f�̌^,�v�f��,�v�f���ʎq�t�^�N���X>*/
		template<typename T,int S,template<typename>class VectorElementIdentifer = vei::DumyIdentifer>
		class Vector : public VectorElementIdentifer<T> {
			/*�T�C�Y��0���傫�����m�F*/
			static_assert(S > 0, "Invalid Vector size. The vector size must be bigger than 0");
			/*�^���Z�p�^���m�F*/
			static_assert(std::is_arithmetic<T>::value == true, "The element type must to be an arithmetic type.");
			/*�����̌^*/
			using MyType = Vector<T, S, VectorElementIdentifer>;
			/*�v�f���ʎq�t�^�N���X�^*/
			using VectorElementIdentiferType = VectorElementIdentifer<T>;
			/*�����̗v�f���ʎq�t�^�N���X�^������Vector�^*/
			template<typename ET,int VS>
			using MyVEIVectorType = Vector<ET, VS, VectorElementIdentifer>;
			/*�v�f���ʎq�t�^�N���X�̍Œ�T�C�Y�𖞂����Ă��邩�m�F*/
			static_assert(S >= VectorElementIdentiferType::minimum_vector_size, "The vector size must not to be less than VectorElementIdentifer::minimum_vector_size.");
		public:
			/*�f�t�H���g�R���X�g���N�^�B�v�f��S��0�ɂ���*/
			Vector() { SetElementArrayPointer(elements_.data()); elements_.fill(0); }
			/*�R�s�[�R���X�g���N�^*/
			Vector(const MyType& v) :elements_(v.elements_) { SetElementArrayPointer(elements_.data()); }
			/*���[�u�R���X�g���N�^(std::array�ŗv�f�����̂Ŗ��Ӗ�)*/
//			Vector(MyType&& v) :elements_(std::move(v.elements_)) {SetElementArrayPointer(elements_.data());}
			/*�ϒ��̒l���Ƃ�R���X�g���N�^(�ÖٓI�ϊ������ۂ���Ə��������X�g�ŏ������ł��Ȃ�)*/
			template<typename... ArgT>
			Vector(const T& first_arg, ArgT... args) :  elements_({ first_arg, args... }) {
				static_assert(S == (sizeof...(ArgT)+1), "The number of constructor argument must equal to Vector size.");
				SetElementArrayPointer(elements_.data());
			}
			/*�T�C�Y��^�̈قȂ�x�N�g���ŏ�����(�ÖٓI�ϊ�������)*/
			template<typename T2,int S2,template<typename>class VEI>
			Vector(const Vector<T2, S2, VEI>& v) {
				SetElementArrayPointer(elements_.data());
				for (int i = 0; i < S; ++i) {
					(*this)[i] = i < S2 ? v[i] : 0;
				}
			}
			//������Z�q
			MyType& operator=(const MyType& v) { elements_ = v.elements_; return *this; }
			//���[�u���Z�q(std::array�ŗv�f�����̂Ŗ��Ӗ�)
//			MyType& operator=(MyType&& v) { elements_ = std::move(v.elements_); return *this; }
			/*�T�C�Y��^�̈قȂ�x�N�g������*/
			template<typename T2, int S2, template<typename>class VEI>
			Vector<T,S>& operator=(const Vector<T2, S2, VEI>& v) {
				for (int i = 0; i < S; ++i) {
					(*this)[i] = static_cast<T>(i < S2 ? v[i] : 0);
				}
				return *this;
			}
			//�������Z�q(�T�C�Y���قȂ�ꍇ�͂��Ԃ镔���݂̂Ŕ�r)
			template<typename T2,int S2, template<typename>class VEI>
			bool operator==(const Vector<T2, S2, VEI>& v)const {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) { if ((*this)[i] != v[i]) { return false; } }
				return true;
			}
			template<typename T2, int S2, template<typename>class VEI>
			bool operator!=(const Vector<T2, S2,VEI>& v)const {
				return !(*this == v);
			}
			//�v�f�ɃA�N�Z�X
			T& operator[](size_t idx) { return elements_[idx]; }
			const T& operator[](size_t idx)const { return elements_[idx]; }
			//�x�N�g���Ƃ̉��Z
			template<typename T2,int S2, template<typename>class VEI>
			const Vector<T,S>& operator+=(const Vector<T2, S2, VEI>& v) {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					(*this)[i] += v[i];
				}
				return *this;
			}
			template<typename T2, int S2, template<typename>class VEI>
			auto operator+(const Vector<T2, S2, VEI>& v)const {
				constexpr int MaxS = S < S2 ? S2 : S;
				MyVEIVectorType<decltype(std::declval<T>() + std::declval<T2>()), MaxS> ret;
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					ret[i] += v[i];
				}
				return std::move(ret);
			}
			//�x�N�g���Ƃ̌��Z
			template<typename T2, int S2, template<typename>class VEI>
			const MyType& operator-=(const Vector<T2, S2, VEI>& v) {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					(*this)[i] -= v[i];
				}
				return *this;
			}
			template<typename T2, int S2, template<typename>class VEI>
			auto operator-(const Vector<T2, S2, VEI>& v)const {
				constexpr int MaxS = S < S2 ? S2 : S;
				MyVEIVectorType<decltype(std::declval<T>() - std::declval<T2>()), MaxS> ret;
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
				MyVEIVectorType<decltype(std::declval<T>()*std::declval<T2>()), S> ret;
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
				MyVEIVectorType<decltype(std::declval<T>()/std::declval<T2>()),S> ret;
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

		private:
			std::array<T,S> elements_;
		};

		//�g�ݍ��݌^�Ƃ̉��Z�q
		template<typename SCALA_T, typename VECTOR_T, int VECTOR_S, template<typename>class VEI>
		auto operator*(const SCALA_T& s, const Vector<VECTOR_T, VECTOR_S,VEI>& v) {
			return v*s;
		}
	}
}