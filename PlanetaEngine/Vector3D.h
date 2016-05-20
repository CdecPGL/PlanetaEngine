#pragma once

#include <cstdint>
#include <string>
#include "MathConstant.h"

namespace planeta_engine {
	namespace math {
		template<typename T>
		struct Vector2D;
		//�O�����x�N�g���e���v���[�g
		template<typename T> struct Vector3D {
		public:
			//�v�f
			T x, y, z;
			//�R���X�g���N�^
			constexpr Vector3D() :x(0), y(0), z(0) {};
			constexpr Vector3D(T m_x, T m_y, T m_z) : x(m_x), y(m_y),z(m_z) {};
			constexpr Vector3D(const Vector3D<T>& obj) :x(obj.x), y(obj.y), z(obj.z) {}
			template<typename T2>
			explicit constexpr Vector3D(const Vector3D<T2>& obj) : x(static_cast<T>(obj.x)), y(static_cast<T>(obj.y)), z(static_cast<T>(obj.z)) {}
			constexpr Vector3D(const Vector2D<T>& v2) : x(v2.x), y(v2.y), z(0) {}
			//������Z�q
			Vector3D<T>& operator =(const Vector3D<T>& obj) { x = obj.x; y = obj.y; z = obj.z; return *this; }
			//�L���X�g���Z�q
			template<typename T2>
			explicit constexpr operator Vector3D<T2>()const { return Vector3D<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z)); }
			//�����Z���Z�q
			constexpr Vector3D<T> operator+(const Vector3D<T>& in)const {
				return Vector3D<T>(x + in.x, y + in.y, z + in.z);
			}
			constexpr Vector3D<T>operator-(const Vector3D<T>& in)const {
				return Vector3D<T>(x - in.x, y - in.y, z - in.z);
			}
			//�X�J���[�Ƃ̏揜�Z���Z�q
			template<typename U>
			constexpr Vector3D<T> operator*(U num)const {
				return Vector3D<T>((T)(x*num), (T)(y*num), (T)(z*num));
			}
			template<typename U>
			Vector3D<T> operator/(U num)const {
				if (num == 0) {
					throw std::range_error("Divided by zero.");
				}
				Vector3D<T> out(static_cast<T>(static_cast<double>(x) / num), static_cast<T>(static_cast<double>(y) / num), static_cast<T>(static_cast<double>(z) / num));
				return out;
			}
			//����������Z�q
			Vector3D<T>& operator+=(const Vector3D<T>& in) {
				x += in.x;
				y += in.y;
				z += in.z;
				return *this;
			}
			Vector3D<T>& operator-=(const Vector3D<T>& in) {
				x -= in.x;
				y -= in.y;
				z -= in.z;
				return *this;
			}
			//�X�J���[�Ƃ̏揜�Z������Z�q
			template<typename U>
			Vector3D<T> operator*=(U num) {
				x *= num;
				y *= num;
				z *= num;
				return *this;
			}
			template<typename U>
			Vector3D<T> operator/=(U num) {
				if (num == 0) {
					throw std::range_error("Divided by zero.");
				}
				x /= num;
				y /= num;
				z /= num;
				return *this;
			}
			//�������]���Z�q
			constexpr Vector3D<T> operator-()const {
				return  Vector3D<T>(-x, -y, -z);
			}
			//��r���Z�q
			constexpr bool operator==(const Vector3D<T>& in)const {
				return in.x == x && in.y == y && in.z == z;
			}
			constexpr bool operator!=(const Vector3D<T>& in)const {
				return !(*this == in);
			}
			//���̑��֐�
			void Set(T mx, T my, T mz) {
				x = mx; y = my; z = mz;
			}
			double length()const {
				return sqrt(length_square());
			}
			constexpr double length_square()const {
				return x*x + y*y + z*z;
			}
			const std::string ToString()const {
				using namespace std;
				stringstream is;
				is << "(" << x << "," << y << "," << z << ")";
				return is.str();
			}

			//��`�ς݃x�N�g��
			static const Vector3D<T> zero_vector;
			static const Vector3D<T> x_unit_vector;
			static const Vector3D<T> y_unit_vector;
			static const Vector3D<T> z_unit_vector;
		};

		template<typename T>
		const Vector3D<T> Vector3D<T>::zero_vector(0, 0, 0);
		template<typename T>
		const Vector3D<T> Vector3D<T>::x_unit_vector(1, 0, 0);
		template<typename T>
		const Vector3D<T> Vector3D<T>::y_unit_vector(0, 1, 0);
		template<typename T>
		const Vector3D<T> Vector3D<T>::z_unit_vector(0, 0, 1);

		//�񎟌��x�N�g���֐�
		//���ς����߂�
		template<typename T>
		constexpr auto Dot(const Vector3D<T>& v1, const Vector3D<T>& v2) {
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}
		//�O�ς����߂�
		/*template<typename T>
		constexpr auto Cross(const Vector3D<T>& v1, const Vector3D<T>& v2) {
			return v1.x*v2.y - v1.y*v2.x;
		}*/
		//���s�ȒP�ʃx�N�g�������߂�(���������̂���)
		template<typename T>
		Vector3D<T> GetParallelUnitVector(const Vector3D<T>& v) {
			double v_length(v.length());
			if (v_length == 0) { throw std::range_error("tryed to calc unit vector from zero vector"); }
			Vector3D<T>out(v / v.length());
			return out;
		}
		//�����P�ʃx�N�g�������߂�(���v����90�x��]��������)
		/*template<typename T>
		Vector3D<T> GetVerticalUnitVector(const Vector3D<T>& v) {
			Vector3D<T> v_vtcl(-v.y, v.x);
			double v_length(v_vtcl.length());
			Vector3D<T> out(GetParallelUnitVector(v_vtcl));
			return out;
		}*/
		//v2��v1�ƂȂ��p�̃R�T�C���l�����߂�(���v���肪��)
		template<typename T>
		double GetCosin(const Vector3D<T>& v1, const Vector3D<T>& v2) {
			double v1_length(v1.length()), v2_length(v2.length());
			double v1v2_dot(Dot(v1, v2));
			return v1v2_dot / (v1_length*v2_length);
		}
		//v2��v1�ƂȂ��p�̃T�C���l�����߂�(���v���肪��)
		/*template<typename T>
		double GetSin(const Vector3D<T>& v1, const Vector3D<T>& v2) {
			double v1_length(v1.length()), v2_length(v2.length());
			double v1v2_cross(Cross(v1, v2));
			return v1v2_cross / (v1_length*v2_length);
		}*/
		//v2��v1�ƂȂ��p�̊p�x(���W�A��)�����߂�(���v���肪��)
		/*template<typename T>
		double GetRadian(const Vector3D<T>& v1, const Vector3D<T>& v2) {
			double v1v2_cos(GetCosin(v1, v2));
			double v1v1_sin(GetSin(v1, v2));
			double v1v2_rad(acos(v1v2_cos));
			if (v1v1_sin < 0) { v1v2_rad = -v1v2_rad; }
			return v1v2_rad;
		}*/
		//v2��v1�ƂȂ��p�̊p�x(�x)�����߂�(���v���肪��)
		/*template<typename T>
		double GetDegree(const Vector3D<T>& v1, const Vector3D<T>& v2) {
			double v1v2_rad = (GetRadian(v1, v2));
			double v1v2_deg(v1v2_rad / math::PI * 180);
			return v1v2_deg;
		}*/
		//v1�̎l�̌ܓ��l�x�N�g�����擾
		template<typename T>
		Vector3D<T> GetRoundVector(const Vector3D<T>& v) {
			return Vector3D<T>((T)round((double)v.x), (T)round((double)v.y), (T)round((double)v.z));
		}
		//v1��v2�ɑ΂��鐳�ˉe�x�N�g�������߂�
		template<typename T>
		Vector3D<T> GetOrthogonalProjectionVector(const Vector3D<T>& v1, const Vector3D<T>& v2) {
			return GetParallelUnitVector(v2) * Vector3D<T>::Dot(v1, v2);
		}
		//�w�肵���p�x�����̒P�ʃx�N�g�����擾����
		/*template<typename T>
		Vector3D<T> GetUnitVectorByRadian(double rad) {
			return Vector3D<T>(static_cast<T>(std::cos(rad)), static_cast<T>(std::sin(rad)));
		}*/
	};
	
	using Vector3Df = math::Vector3D<float>; //�P���x���������_�^�O�����x�N�g��
	using Vector3Dd = math::Vector3D<double>; //�{���x���������_�^�O�����x�N�g��
	using Vector3Di = math::Vector3D<int32_t>; //32bit�����t�������^�O�����x�N�g��
}