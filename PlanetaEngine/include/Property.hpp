#pragma once

#include <type_traits>

namespace plnt {
	namespace util {
		/*読み取り専用プロパティ*/
		template<class C, typename PropertyType, typename GetterRet, GetterRet(C::*getter)()const>
		class ReadOnlyProperty {
			using MyType = ReadOnlyProperty<C, PropertyType, GetterRet, getter>;
		public:
			explicit ReadOnlyProperty(const C& ins) :instance_(ins) {}
			//プロパティ型への変換
			operator PropertyType()const {
				return (instance_.*getter)();
			}
			/*()を用いたアクセス*/
			GetterRet operator()()const& {
				return get();
			}
			PropertyType&& operator()()const&& {
				PropertyType ret = get();
				return std::move(ret);
			}
			//四則演算子
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
			//ビット演算子
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
			//シフト演算
			template<typename ROP>
			auto operator>>(const ROP& right_op)const {
				return get() >> right_op;
			}
			template<typename ROP>
			auto operator<<(const ROP& right_op)const {
				return get() << right_op;
			}
			//単項演算子
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
		/*書き込み専用プロパティ*/
		template<class C, typename PropertyType, typename SetterArg, void(C::*setter)(SetterArg)>
		class WriteOnlyProperty {
			using MyType = WriteOnlyProperty<C, PropertyType, SetterArg, setter>;
		public:
			explicit WriteOnlyProperty(C& ins) :instance_(ins) {}
			//代入
			MyType& operator=(SetterArg v) {
				(instance_.*setter)(v);
				return *this;
			}
			/*()を用いたアクセス*/
			void operator()(SetterArg v) {
				set(v);
			}
		protected:
			void set(SetterArg v) { (instance_.*setter)(v); }
		private:
			C& instance_;
		};
		
		/*読み書きプロパティ*/
		template<class C,typename PropertyType,typename GetterRet,typename SetterArg, GetterRet(C::*getter)()const, void (C::*setter)(SetterArg)>
		class ReadWriteProperty :
			public ReadOnlyProperty<C,PropertyType,GetterRet,getter>,
			public WriteOnlyProperty<C,PropertyType,SetterArg,setter>{
			using MyType = ReadWriteProperty<C, PropertyType,GetterRet,SetterArg, getter, setter>;
			using ReadPropertyType = ReadOnlyProperty<C, PropertyType, GetterRet, getter>;
			using WritePropertyType = WriteOnlyProperty<C, PropertyType, SetterArg, setter>;
		public:
			explicit ReadWriteProperty(C& ins) :ReadPropertyType(ins), WritePropertyType(ins) {}
			//代入
			MyType& operator=(SetterArg v) {
				WritePropertyType::operator =(v);
				return *this;
			}
			//前置インクリメント
			MyType& operator++() {
				PropertyType buf = get();
				set(++buf);
				return *this;
			}
			//後置インクリメント
			PropertyType operator++(int) {
				PropertyType buf = get();
				PropertyType ret = buf++;
				set(buf);
				return ret;
			}
			//前置デクリメント
			MyType& operator--() {
				PropertyType buf = get();
				set(--buf);
				return *this;
			}
			//後置デクリメント
			PropertyType operator--(int) {
				PropertyType buf = get();
				PropertyType ret = buf--;
				set(buf);
				return ret;
			}
			//四則演算
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
			//ビット演算
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
			//シフト演算
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
		//getter、setterで値渡しを用いる読み書きプロパティ
		template<class C,typename PropertyType,PropertyType(C::*getter)()const,void(C::*setter)(PropertyType)>
		using ReadWritePropertyV = ReadWriteProperty<C, PropertyType, PropertyType, PropertyType, getter, setter>;
		//getter、setterで値渡しを用いる読み取り専用プロパティ
		template<class C, typename PropertyType, PropertyType(C::*getter)()const>
		using ReadOnlyPropertyV = ReadOnlyProperty<C, PropertyType, PropertyType, getter>;
		//getter、setterで参照渡しを用いる読み書きプロパティ
		template<class C, typename PropertyType, const PropertyType&(C::*getter)()const, void(C::*setter)(const PropertyType&)>
		using ReadWritePropertyR = ReadWriteProperty<C, PropertyType, const PropertyType&, const PropertyType&, getter, setter>;
		//getter、setterで参照渡しを用いる読み取り専用プロパティ
		template<class C, typename PropertyType, const PropertyType&(C::*getter)()const>
		using ReadOnlyPropertyR = ReadOnlyProperty<C, PropertyType, const PropertyType&, getter>;
	}
}