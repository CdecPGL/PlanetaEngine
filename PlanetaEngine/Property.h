#pragma once

#include <type_traits>

namespace planeta_engine {
	namespace utility {
		/*�g�ݍ��݌^(�Q�Ǝ����K�v�Ȃ��^)�p�ǂݏ����v���p�e�B(�X�J���^�݂̂ɑΉ�)*/
		template<class C,typename PropertyType, PropertyType(C::*getter)()const, void (C::*setter)(PropertyType)>
		class Property {
			static_assert(std::is_scalar<PropertyType>::value == true, "PropertyType must be a scala type.");
			using MyType = Property<C, PropertyType, getter, setter>;
		public:
			Property(C& ins) :instance_(ins) {}
			//���
			MyType& operator=(PropertyType v) {
				(instance_.*setter)(v);
				return *this;
			}
			//�v���p�e�B�^�ւ̕ϊ�
			operator PropertyType()const {
				return (instance_.*getter)();
			}
			/*()��p�����A�N�Z�X*/
			PropertyType operator()()const {
				return get();
			}
			void operator()(PropertyType v) {
				set(v);
			}
			//�O�u�C���N�������g(���Z�ōČ�)
			MyType& operator++() {
				set(get() + 1);
				return *this;
			}
			//��u�C���N�������g(���Z�ōČ�)
			PropertyType operator++(int) {
				PropertyType ret = get();
				set(ret + 1);
				return ret;
			}
			//�O�u�f�N�������g(���Z�ōČ�)
			MyType& operator--() {
				set(get() - 1);
				return *this;
			}
			//��u�f�N�������g(���Z�ōČ�)
			PropertyType operator--(int) {
				PropertyType ret = get();
				set(ret - 1);
				return ret;
			}
			//�l�����Z
			template<typename ROP>
			MyType& operator+=(const ROP& rop){
				set(get() + rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator-=(const ROP& rop) {
				set(get() - rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator*=(const ROP& rop) {
				set(get() * rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator/=(const ROP& rop) {
				set(get() / rop);
				return *this;
			}
			//�r�b�g���Z
			template<typename ROP>
			MyType& operator|=(const ROP& rop) {
				set(get() | rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator&=(const ROP& rop) {
				set(get() & rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator^=(const ROP& rop) {
				set(get() ^ rop);
				return *this;
			}
			//�V�t�g���Z
			template<typename ROP>
			MyType& operator>>=(const ROP& rop) {
				set(get() >> rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator<<=(const ROP& rop) {
				set(get() << rop);
				return *this;
			}
		private:
			void set(PropertyType v){ (instance_.*setter)(v); }
			PropertyType get()const { return (instance_.*getter)(); }
			C& instance_;
		};
		/*�g�ݍ��݌^(�Q�Ǝ����K�v�Ȃ��^)�p�ǂݎ���p�v���p�e�B*/
		//template<class C, typename PropertyType, PropertyType(C::*getter)()const>
		//class Property {
		//	using MyType = Property<C, PropertyType, getter>;
		//public:
		//	Property(C& ins) :instance_(ins) {}
		//	//�v���p�e�B�^�ւ̕ϊ�
		//	operator PropertyType()const {
		//		return (instance_.*getter)();
		//	}
		//private:
		//	PropertyType get()const { return (instance_.*getter)(); }
		//	C& instance_;
		//};
	}
}