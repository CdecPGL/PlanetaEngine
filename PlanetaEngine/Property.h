#pragma once

#include <type_traits>

namespace planeta_engine {
	namespace utility {
		/*組み込み型(参照私が必要ない型)用読み書きプロパティ(スカラ型のみに対応)*/
		template<class C,typename PropertyType, PropertyType(C::*getter)()const, void (C::*setter)(PropertyType)>
		class Property {
			static_assert(std::is_scalar<PropertyType>::value == true, "PropertyType must be a scala type.");
			using MyType = Property<C, PropertyType, getter, setter>;
		public:
			Property(C& ins) :instance_(ins) {}
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
				PropertyType ret = get();
				set(ret + 1);
				return ret;
			}
			//前置デクリメント(減算で再現)
			MyType& operator--() {
				set(get() - 1);
				return *this;
			}
			//後置デクリメント(減算で再現)
			PropertyType operator--(int) {
				PropertyType ret = get();
				set(ret - 1);
				return ret;
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
		//class Property {
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
	}
}