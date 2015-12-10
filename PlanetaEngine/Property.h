#pragma once

#include <type_traits>

namespace planeta_engine {
	namespace utility {
		/*組み込み型(参照私が必要ない型)用読み書きプロパティ(スカラ型のみに対応)*/
		template<class C,typename PropertyType, PropertyType(C::*getter)()const, void (C::*setter)(PropertyType)>
		class PropertyV {
			static_assert(std::is_scalar<PropertyType>::value == true, "PropertyType must be a scala type.");
			using MyType = PropertyV<C, PropertyType, getter, setter>;
		public:
			PropertyV(C& ins) :instance_(ins) {}
			//代入
			MyType& operator=(PropertyType v) {
				(instance_.*setter)(v);
				return *this;
			}
			//プロパティ型への変換
			operator PropertyType()const {
				return (instance_.*getter)();
			}
			/*()を用いたアクセス*/
			PropertyType operator()()const {
				return get();
			}
			void operator()(PropertyType v) {
				set(v);
			}
			//前置インクリメント(加算で再現)
			MyType& operator++() {
				set(get() + 1);
				return *this;
			}
			//後置インクリメント(加算で再現)
			PropertyType operator++(int) {
				PropertyType buf = get();
				set(buf + 1);
				return buf;
			}
			//前置デクリメント(減算で再現)
			MyType& operator--() {
				set(get() - 1);
				return *this;
			}
			//後置デクリメント(減算で再現)
			PropertyType operator--(int) {
				PropertyType buf = get();
				set(buf - 1);
				return buf;
			}
			//四則演算
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
			//ビット演算
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
			//シフト演算
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
		/*組み込み型(参照私が必要ない型)用読み取り専用プロパティ*/
		//template<class C, typename PropertyType, PropertyType(C::*getter)()const>
		//class PropertyV {
		//	using MyType = Property<C, PropertyType, getter>;
		//public:
		//	Property(C& ins) :instance_(ins) {}
		//	//プロパティ型への変換
		//	operator PropertyType()const {
		//		return (instance_.*getter)();
		//	}
		//private:
		//	PropertyType get()const { return (instance_.*getter)(); }
		//	C& instance_;
		//};
		/*複合型(参照渡しが必要な型)用読み書きプロパティ*/
		template<class C, typename PropertyType, const PropertyType&(C::*getter)()const, void (C::*setter)(const PropertyType&)>
		class PropertyR {
			using MyType = PropertyR<C, PropertyType, getter, setter>;
		public:
			PropertyR(C& ins) :instance_(ins) {}
			//代入
			MyType& operator=(const PropertyType& v) {
				(instance_.*setter)(v);
				return *this;
			}
			//プロパティ型への変換
			operator PropertyType()const {
				return (instance_.*getter)();
			}
			/*()を用いたアクセス*/
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
			//前置インクリメント(加算で再現)
			MyType& operator++() {
				PropertyType buf = get();
				set(++buf);
				return *this;
			}
			//後置インクリメント(加算で再現)
			PropertyType operator++(int) {
				PropertyType buf = get();
				PropertyType ret = buf++;
				set(buf);
				return ret;
			}
			//前置デクリメント(減算で再現)
			MyType& operator--() {
				PropertyType buf = get();
				set(--buf);
				return *this;
			}
			//後置デクリメント(減算で再現)
			PropertyType operator--(int) {
				PropertyType buf = get();
				PropertyType ret = buf--;
				set(buf);
				return ret;
			}
			//四則演算
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
		private:
			void set(const PropertyType& v) { (instance_.*setter)(v); }
			const PropertyType& get()const { return (instance_.*getter)(); }
			C& instance_;
		};
	}
}