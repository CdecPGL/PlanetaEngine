#pragma once

#include <type_traits>

namespace planeta_engine {
	namespace utility {
		/*�g�ݍ��݌^(�Q�Ǝ����K�v�Ȃ��^)�p�ǂݏ����v���p�e�B(�X�J���^�݂̂ɑΉ�)*/
		template<class C,typename PropertyType, PropertyType(C::*getter)()const, void (C::*setter)(PropertyType)>
		class PropertyV {
			static_assert(std::is_scalar<PropertyType>::value == true, "PropertyType must be a scala type.");
			using MyType = PropertyV<C, PropertyType, getter, setter>;
		public:
			PropertyV(C& ins) :instance_(ins) {}
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
				PropertyType buf = get();
				set(buf + 1);
				return buf;
			}
			//�O�u�f�N�������g(���Z�ōČ�)
			MyType& operator--() {
				set(get() - 1);
				return *this;
			}
			//��u�f�N�������g(���Z�ōČ�)
			PropertyType operator--(int) {
				PropertyType buf = get();
				set(buf - 1);
				return buf;
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
		//class PropertyV {
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
		/*�����^(�Q�Ɠn�����K�v�Ȍ^)�p�ǂݏ����v���p�e�B*/
		template<class C, typename PropertyType, const PropertyType&(C::*getter)()const, void (C::*setter)(const PropertyType&)>
		class PropertyR {
			using MyType = PropertyR<C, PropertyType, getter, setter>;
		public:
			PropertyR(C& ins) :instance_(ins) {}
			//���
			MyType& operator=(const PropertyType& v) {
				(instance_.*setter)(v);
				return *this;
			}
			//�v���p�e�B�^�ւ̕ϊ�
			operator PropertyType()const {
				return (instance_.*getter)();
			}
			/*()��p�����A�N�Z�X*/
			const PropertyType& operator()()const& {
				return get();
			}
			PropertyType&& operator()()&& {
				T ret = get();
				return std::move(ret);
			}
			void operator()(const PropertyType& v) {
				set(v);
			}
			//�O�u�C���N�������g(���Z�ōČ�)
			MyType& operator++() {
				PropertyType buf = get();
				set(++buf);
				return *this;
			}
			//��u�C���N�������g(���Z�ōČ�)
			PropertyType operator++(int) {
				PropertyType buf = get();
				PropertyType ret = buf++;
				set(buf);
				return ret;
			}
			//�O�u�f�N�������g(���Z�ōČ�)
			MyType& operator--() {
				PropertyType buf = get();
				set(--buf);
				return *this;
			}
			//��u�f�N�������g(���Z�ōČ�)
			PropertyType operator--(int) {
				PropertyType buf = get();
				PropertyType ret = buf--;
				set(buf);
				return ret;
			}
			//�l�����Z
			template<typename ROP>
			MyType& operator+=(const ROP& rop) {
				PropertyType buf = get();
				set(buf += rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator-=(const ROP& rop) {
				PropertyType buf = get();
				set(buf -= rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator*=(const ROP& rop) {
				PropertyType buf = get();
				set(buf *= rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator/=(const ROP& rop) {
				PropertyType buf = get();
				set(buf /= rop);
				return *this;
			}
			//�r�b�g���Z
			template<typename ROP>
			MyType& operator|=(const ROP& rop) {
				PropertyType buf = get();
				set(buf |= rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator&=(const ROP& rop) {
				PropertyType buf = get();
				set(buf &= rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator^=(const ROP& rop) {
				PropertyType buf = get();
				set(buf ^= rop);
				return *this;
			}
			//�V�t�g���Z
			template<typename ROP>
			MyType& operator>>=(const ROP& rop) {
				PropertyType buf = get();
				set(buf >>= rop);
				return *this;
			}
			template<typename ROP>
			MyType& operator<<=(const ROP& rop) {
				PropertyType buf = get();
				set(buf <<= rop);
				return *this;
			}
		private:
			void set(const PropertyType& v) { (instance_.*setter)(v); }
			const PropertyType& get()const { return (instance_.*getter)(); }
			C& instance_;
		};
	}
}