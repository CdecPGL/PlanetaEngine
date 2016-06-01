#pragma once

#include <type_traits>

namespace planeta {
	namespace util {
		/*�ǂݎ���p�v���p�e�B*/
		template<class C, typename PropertyType, typename GetterRet, GetterRet(C::*getter)()const>
		class ReadOnlyProperty {
			using MyType = ReadOnlyProperty<C, PropertyType, GetterRet, getter>;
		public:
			explicit ReadOnlyProperty(const C& ins) :instance_(ins) {}
			//�v���p�e�B�^�ւ̕ϊ�
			operator PropertyType()const {
				return (instance_.*getter)();
			}
			/*()��p�����A�N�Z�X*/
			GetterRet operator()()const& {
				return get();
			}
			PropertyType&& operator()()const&& {
				PropertyType ret = get();
				return std::move(ret);
			}
			//�l�����Z�q
			template<typename ROP>
			auto operator+(const ROP& right_op)const {
				return get() + right_op;
			}
			template<typename ROP>
			auto operator-(const ROP& right_op)const {
				return get() - right_op;
			}
			template<typename ROP>
			auto operator*(const ROP& right_op)const {
				return get() * right_op;
			}
			template<typename ROP>
			auto operator/(const ROP& right_op)const {
				return get() / right_op;
			}
			//�r�b�g���Z�q
			template<typename ROP>
			auto operator|(const ROP& right_op)const {
				return get() | right_op;
			}
			template<typename ROP>
			auto operator&(const ROP& right_op)const {
				return get() & right_op;
			}
			template<typename ROP>
			auto operator^(const ROP& right_op)const {
				return get() ^ right_op;
			}
			//�V�t�g���Z
			template<typename ROP>
			auto operator>>(const ROP& right_op)const {
				return get() >> right_op;
			}
			template<typename ROP>
			auto operator<<(const ROP& right_op)const {
				return get() << right_op;
			}
			//�P�����Z�q
			auto operator+()const {
				return +get();
			}
			auto operator-()const {
				return -get();
			}
			auto operator!()const {
				return !get();
			}
		protected:
			GetterRet get()const { return (instance_.*getter)(); }
		private:
			const C& instance_;
		};
		/*�������ݐ�p�v���p�e�B*/
		template<class C, typename PropertyType, typename SetterArg, void(C::*setter)(SetterArg)>
		class WriteOnlyProperty {
			using MyType = WriteOnlyProperty<C, PropertyType, SetterArg, setter>;
		public:
			explicit WriteOnlyProperty(C& ins) :instance_(ins) {}
			//���
			MyType& operator=(SetterArg v) {
				(instance_.*setter)(v);
				return *this;
			}
			/*()��p�����A�N�Z�X*/
			void operator()(SetterArg v) {
				set(v);
			}
		protected:
			void set(SetterArg v) { (instance_.*setter)(v); }
		private:
			C& instance_;
		};
		
		/*�ǂݏ����v���p�e�B*/
		template<class C,typename PropertyType,typename GetterRet,typename SetterArg, GetterRet(C::*getter)()const, void (C::*setter)(SetterArg)>
		class ReadWriteProperty :
			public ReadOnlyProperty<C,PropertyType,GetterRet,getter>,
			public WriteOnlyProperty<C,PropertyType,SetterArg,setter>{
			using MyType = ReadWriteProperty<C, PropertyType,GetterRet,SetterArg, getter, setter>;
			using ReadPropertyType = ReadOnlyProperty<C, PropertyType, GetterRet, getter>;
			using WritePropertyType = WriteOnlyProperty<C, PropertyType, SetterArg, setter>;
		public:
			explicit ReadWriteProperty(C& ins) :ReadPropertyType(ins), WritePropertyType(ins) {}
			//���
			MyType& operator=(SetterArg v) {
				WritePropertyType::operator =(v);
				return *this;
			}
			//�O�u�C���N�������g
			MyType& operator++() {
				PropertyType buf = get();
				set(++buf);
				return *this;
			}
			//��u�C���N�������g
			PropertyType operator++(int) {
				PropertyType buf = get();
				PropertyType ret = buf++;
				set(buf);
				return ret;
			}
			//�O�u�f�N�������g
			MyType& operator--() {
				PropertyType buf = get();
				set(--buf);
				return *this;
			}
			//��u�f�N�������g
			PropertyType operator--(int) {
				PropertyType buf = get();
				PropertyType ret = buf--;
				set(buf);
				return ret;
			}
			//�l�����Z
			template<typename ROP>
			MyType& operator+=(const ROP& rop){
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
		};
		//getter�Asetter�Œl�n����p����ǂݏ����v���p�e�B
		template<class C,typename PropertyType,PropertyType(C::*getter)()const,void(C::*setter)(PropertyType)>
		using ReadWritePropertyV = ReadWriteProperty<C, PropertyType, PropertyType, PropertyType, getter, setter>;
		//getter�Asetter�Œl�n����p����ǂݎ���p�v���p�e�B
		template<class C, typename PropertyType, PropertyType(C::*getter)()const>
		using ReadOnlyPropertyV = ReadOnlyProperty<C, PropertyType, PropertyType, getter>;
		//getter�Asetter�ŎQ�Ɠn����p����ǂݏ����v���p�e�B
		template<class C, typename PropertyType, const PropertyType&(C::*getter)()const, void(C::*setter)(const PropertyType&)>
		using ReadWritePropertyR = ReadWriteProperty<C, PropertyType, const PropertyType&, const PropertyType&, getter, setter>;
		//getter�Asetter�ŎQ�Ɠn����p����ǂݎ���p�v���p�e�B
		template<class C, typename PropertyType, const PropertyType&(C::*getter)()const>
		using ReadOnlyPropertyR = ReadOnlyProperty<C, PropertyType, const PropertyType&, getter>;
	}
}