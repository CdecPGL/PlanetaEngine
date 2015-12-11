#include "stdafx.h"
#include "CppUnitTest.h"
#include "Property.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlanetaEngineTest {
	using namespace planeta_engine::utility;
	
	TEST_CLASS(PropertyTest) {
	public:

		TEST_METHOD(PropertyVTest) {
			class Test {
			private:
				int v_;
				int get_v()const { return v_; }
				void set_v(int n) { v_ = n; }
			public:
				Test() :v(*this) {}
				ReadWritePropertyV<Test, int, &Test::get_v, &Test::set_v> v;
			} t;
			int pv = 123; t.v = 123;
			Assert::IsTrue(pv == t.v, L"������Z�q�G���[");
			Assert::IsTrue(++pv == ++t.v, L"�O�u�C���N�������g�G���[");
			Assert::IsTrue(pv++ == t.v++, L"��u�C���N�������g�G���[");
			Assert::IsTrue(--pv == --t.v, L"�O�u�f�N�������g�G���[");
			Assert::IsTrue(pv-- == t.v--, L"��u�f�N�������g�G���[");
			pv += 123; t.v += 123;
			Assert::IsTrue(pv == t.v, L"+=���Z�q�G���[");
			pv -= 123; t.v -= 123;
			Assert::IsTrue(pv == t.v, L"-=���Z�q�G���[");
			pv *= 123; t.v *= 123;
			Assert::IsTrue(pv == t.v, L"*=���Z�q�G���[");
			pv /= 123; t.v /= 123;
			Assert::IsTrue(pv == t.v, L"/=���Z�q�G���[");
			pv |= 35; t.v |= 35;
			Assert::IsTrue(pv == t.v, L"|=���Z�q�G���[");
			pv &= 657; t.v &= 657;
			Assert::IsTrue(pv == t.v, L"&=���Z�q�G���[");
			pv ^=634632352; t.v ^= 634632352;
			Assert::IsTrue(pv == t.v, L"^=���Z�q�G���[");
			pv <<= 5; t.v <<= 5;
			Assert::IsTrue(pv == t.v, L"<<=���Z�q�G���[");
			pv >>= 5; t.v >>= 5;
			Assert::IsTrue(pv == t.v, L">>=���Z�q�G���[");
		}

		TEST_METHOD(PropertyRTest) {
			class Value {
			public:
				Value() :Value(0, 0) {}
				Value(int x, int y):x_(x),y_(y) {}
				bool operator==(const Value& v) {
					return x_ == v.x_ && y_ == v.y_;
				}
				Value& operator++() {
					++x_;
					++y_;
					return *this;
				}
				Value operator++(int) {
					Value buf = *this;
					++x_;
					++y_;
					return buf;
				}
				Value& operator--() {
					--x_;
					--y_;
					return *this;
				}
				Value operator--(int) {
					Value buf = *this;
					--x_;
					--y_;
					return buf;
				}
				Value& operator+=(const Value& v) {
					x_ += v.x_;
					y_ += v.y_;
					return *this;
				}
				Value& operator-=(const Value& v) {
					x_ -= v.x_;
					y_ -= v.y_;
					return *this;
				}
				Value& operator*=(const Value& v) {
					x_ *= v.x_;
					y_ *= v.y_;
					return *this;
				}
				Value& operator/=(const Value& v) {
					x_ /= v.x_;
					y_ /= v.y_;
					return *this;
				}
				Value& operator|=(const Value& v) {
					x_ |= v.x_;
					y_ |= v.y_;
					return *this;
				}
				Value& operator&=(const Value& v) {
					x_ &= v.x_;
					y_ &= v.y_;
					return *this;
				}
				Value& operator^=(const Value& v) {
					x_ ^= v.x_;
					y_ ^= v.y_;
					return *this;
				}
				Value& operator>>=(const Value& v) {
					x_ >>= v.x_;
					y_ >>= v.y_;
					return *this;
				}
				Value& operator<<=(const Value& v) {
					x_ <<= v.x_;
					y_ <<= v.y_;
					return *this;
				}
			private:
				int x_;
				int y_;
			};
			class Test {
			private:
				Value v_;
				const Value& get_v()const { return v_; }
				void set_v(const Value& n) { v_ = n; }
			public:
				Test() :v(*this) {}
				ReadWritePropertyR<Test, Value, &Test::get_v, &Test::set_v> v;
			} t;
			Value pv = Value(45, 67); t.v = Value(45,67);
			Assert::IsTrue(pv == t.v, L"������Z�q�G���[");
			Assert::IsTrue(++pv == ++t.v, L"�O�u�C���N�������g�G���[");
			Assert::IsTrue(pv++ == t.v++, L"��u�C���N�������g�G���[");
			Assert::IsTrue(--pv == --t.v, L"�O�u�f�N�������g�G���[");
			Assert::IsTrue(pv-- == t.v--, L"��u�f�N�������g�G���[");
			pv += Value(67,89); t.v += Value(67, 89);
			Assert::IsTrue(pv == t.v, L"+=���Z�q�G���[");
			pv -= Value(67, 89); t.v -= Value(67, 89);
			Assert::IsTrue(pv == t.v, L"-=���Z�q�G���[");
			pv *= Value(67, 89); t.v *= Value(67, 89);
			Assert::IsTrue(pv == t.v, L"*=���Z�q�G���[");
			pv /= Value(67, 89); t.v /= Value(67, 89);
			Assert::IsTrue(pv == t.v, L"/=���Z�q�G���[");
			pv |= Value(4567, 2352); t.v |= Value(4567, 2352);
			Assert::IsTrue(pv == t.v, L"|=���Z�q�G���[");
			pv &= Value(4523567, 2323552); t.v &= Value(4523567, 2323552);
			Assert::IsTrue(pv == t.v, L"&=���Z�q�G���[");
			pv ^= Value(3463, 34622); t.v ^= Value(3463, 34622);
			Assert::IsTrue(pv == t.v, L"^=���Z�q�G���[");
			pv <<= Value(5, 6); t.v <<= Value(5, 6);
			Assert::IsTrue(pv == t.v, L"<<=���Z�q�G���[");
			pv >>= Value(5, 6); t.v >>= Value(5, 6);
			Assert::IsTrue(pv == t.v, L">>=���Z�q�G���[");
		}
	};
}