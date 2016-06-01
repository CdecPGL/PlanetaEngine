#pragma once

#include <cstdint>
#include <string>
#include "MathConstant.h"

namespace planeta {
	namespace math {
		template<typename T>
		struct Vector3D;
		//�񎟌��x�N�g���e���v���[�g
		template<typename T> struct Vector2D {
		public:
			//�v�f
			T x, y;
			//�R���X�g���N�^
			constexpr Vector2D() :x(0), y(0) {};
			constexpr Vector2D(T m_x, T m_y) : x(m_x), y(m_y) {};
			constexpr Vector2D(const Vector2D<T>& obj) :x(obj.x), y(obj.y) {}
			template<typename T2>
			explicit constexpr Vector2D(const Vector2D<T2>& obj) : x(static_cast<T>(obj.x)), y(static_cast<T>(obj.y)) {}
			constexpr Vector2D(const Vector3D<T>& v3):x(v3.x), y(v3.y) {}
			//������Z�q
			Vector2D<T>& operator =(const Vector2D<T>& obj) { x = obj.x; y = obj.y; return *this; }
			//�L���X�g���Z�q
			template<typename T2>
			explicit constexpr operator Vector2D<T2>()const { return Vector2D<T2>(static_cast<T2>(x), static_cast<T2>(y)); }
			//�����Z���Z�q
			constexpr Vector2D<T> operator+(const Vector2D<T>& in)const {
				return Vector2D<T>(x + in.x, y + in.y);
			}
			constexpr Vector2D<T>operator-(const Vector2D<T>& in)const {
				return Vector2D<T>(x - in.x, y - in.y);
			}
			//�X�J���[�Ƃ̏揜�Z���Z�q
			template<typename U>
			constexpr Vector2D<T> operator*(U num)const {
				return Vector2D<T>((T)(x*num), (T)(y*num));
			}
			template<typename U>
			Vector2D<T> operator/(U num)const {
				if (num == 0) {
					throw std::range_error("Divided by zero.");
				}
				Vector2D<T> out(static_cast<T>(static_cast<double>(x) / num), static_cast<T>(static_cast<double>(y) / num));
				return out;
			}
			//����������Z�q
			Vector2D<T>& operator+=(const Vector2D<T>& in) {
				x += in.x;
				y += in.y;
				return *this;
			}
			Vector2D<T>& operator-=(const Vector2D<T>& in) {
				x -= in.x;
				y -= in.y;
				return *this;
			}
			//�X�J���[�Ƃ̏揜�Z������Z�q
			template<typename U>
			Vector2D<T> operator*=(U num) {
				x *= num;
				y *= num;
				return *this;
			}
			template<typename U>
			Vector2D<T> operator/=(U num) {
				if (num == 0) {
					throw std::range_error("Divided by zero.");
				}
				x /= num;
				y /= num;
				return *this;
			}
			//�������]���Z�q
			constexpr Vector2D<T> operator-()const {
				return  Vector2D<T>(-x, -y);
			}
			//��r���Z�q
			constexpr bool operator==(const Vector2D<T>& in)const {
				return in.x == x && in.y == y;
			}
			constexpr bool operator!=(const Vector2D<T>& in)const {
				return !(*this == in);
			}
			//���̑��֐�
			void Set(T mx, T my) {
				x = mx; y = my;
			}
			double length()const {
				return sqrt(length_square());
			}
			constexpr double length_square()const {
				return x*x + y*y;
			}
			const std::string ToString()const {
				using namespace std;
				stringstream is;
				is << "(" << x << "," << y << ")";
				return is.str();
			}

			//��`�ς݃x�N�g��
			static const Vector2D<T> zero_vector;
			static const Vector2D<T> x_unit_vector;
			static const Vector2D<T> y_unit_vector;
		};

		template<typename T>
		const Vector2D<T> Vector2D<T>::zero_vector(0, 0);
		template<typename T>
		const Vector2D<T> Vector2D<T>::x_unit_vector(1, 0);
		template<typename T>
		const Vector2D<T> Vector2D<T>::y_unit_vector(0, 1);

		//�񎟌��x�N�g���֐�
		//���ς����߂�
		template<typename T>
		constexpr auto Dot(const Vector2D<T>& v1, const Vector2D<T>& v2) {
			return v1.x*v2.x + v1.y*v2.y;
		}
		//�O�ς����߂�
		template<typename T>
		constexpr auto Cross(const Vector2D<T>& v1, const Vector2D<T>& v2) {
			return v1.x*v2.y - v1.y*v2.x;
		}
		//���s�ȒP�ʃx�N�g�������߂�(���������̂���)
		template<typename T>
		Vector2D<T> GetParallelUnitVector(const Vector2D<T>& v) {
			double v_length(v.length());
			if (v_length == 0) { throw std::range_error("tryed to calc unit vector from zero vector"); }
			Vector2D<T>out(v / v.length());
			return out;
		}
		//�����P�ʃx�N�g�������߂�(���v����90�x��]��������)
		template<typename T>
		Vector2D<T> GetVerticalUnitVector(const Vector2D<T>& v) {
			Vector2D<T> v_vtcl(-v.y, v.x);
			double v_length(v_vtcl.length());
			Vector2D<T> out(GetParallelUnitVector(v_vtcl));
			return out;
		}
		//v2��v1�ƂȂ��p�̃R�T�C���l�����߂�(���v���肪��)
		template<typename T>
		double GetCosin(const Vector2D<T>& v1, const Vector2D<T>& v2) {
			double v1_length(v1.length()), v2_length(v2.length());
			double v1v2_dot(Dot(v1, v2));
			return v1v2_dot / (v1_length*v2_length);
		}
		//v2��v1�ƂȂ��p�̃T�C���l�����߂�(���v���肪��)
		template<typename T>
		double GetSin(const Vector2D<T>& v1, const Vector2D<T>& v2) {
			double v1_length(v1.length()), v2_length(v2.length());
			double v1v2_cross(Cross(v1, v2));
			return v1v2_cross / (v1_length*v2_length);
		}
		//v2��v1�ƂȂ��p�̊p�x(���W�A��)�����߂�(���v���肪��)
		template<typename T>
		double GetRadian(const Vector2D<T>& v1, const Vector2D<T>& v2) {
			double v1v2_cos(GetCosin(v1, v2));
			double v1v1_sin(GetSin(v1, v2));
			double v1v2_rad(acos(v1v2_cos));
			if (v1v1_sin < 0) { v1v2_rad = -v1v2_rad; }
			return v1v2_rad;
		}
		//v2��v1�ƂȂ��p�̊p�x(�x)�����߂�(���v���肪��)
		template<typename T>
		double GetDegree(const Vector2D<T>& v1, const Vector2D<T>& v2) {
			double v1v2_rad = (GetRadian(v1, v2));
			double v1v2_deg(v1v2_rad / math::PI * 180);
			return v1v2_deg;
		}
		//v1�̎l�̌ܓ��l�x�N�g�����擾
		template<typename T>
		Vector2D<T> GetRoundVector(const Vector2D<T>& v) {
			return Vector2D<T>((T)round((double)v.x), (T)round((double)v.y));
		}
		//v1��v2�ɑ΂��鐳�ˉe�x�N�g�������߂�
		template<typename T>
		Vector2D<T> GetOrthogonalProjectionVector(const Vector2D<T>& v1, const Vector2D<T>& v2) {
			return GetParallelUnitVector(v2) * Vector2D<T>::Dot(v1, v2);
		}
		//�w�肵���p�x�����̒P�ʃx�N�g�����擾����
		template<typename T>
		Vector2D<T> GetUnitVectorByRadian(double rad) {
			return Vector2D<T>(static_cast<T>(std::cos(rad)), static_cast<T>(std::sin(rad)));
		}
	}

	//�񎟌��x�N�g���^��`
	using Vector2Df = math::Vector2D<float>; //�P���x���������_�^�񎟌��x�N�g��
	using Vector2Dd = math::Vector2D<double>; //�{���x���������_�^�񎟌��x�N�g��
	using Vector2Di = math::Vector2D<int32_t>; //32bit�����t�������^�񎟌��x�N�g��

}
