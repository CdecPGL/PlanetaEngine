#include "stdafx.h"
#include "CppUnitTest.h"
#include "PETime.h"
#include "WeakPointer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlanetaEngineTest
{
	using namespace planeta;
	using namespace planeta::util;
	class Base {
	public:
		virtual ~Base() = default;
		int x;
	};
	class Child : public Base {

	};
	TEST_CLASS(UtilityTest)
	{
	public:

		TEST_METHOD(TimeTest)
		{
			Time a(10, 10, 10), b(5, 20, 34);
			Assert::IsTrue(a == a, L"==演算子の結果が異常");
			Assert::IsFalse(a != a, L"!=演算子の結果が異常");
			Assert::IsTrue(a + b == Time(15, 30, 44), L"時間の加算結果が異常");
			Assert::IsTrue(a - b == Time(4, 49, 36), L"時間の減算結果が異常");
			Assert::IsTrue(b*3 == Time(16, 1, 42), L"時間の整数倍結果が異常");
			Assert::IsTrue((a > b) && !(a > a) , L">演算子の結果が異常");
			Assert::IsTrue(!(a < b) && !(a < a), L"<演算子の結果が異常");
			Assert::IsTrue((a >= b) && (a >= a), L">=演算子の結果が異常");
			Assert::IsTrue(!(a <= b) && (a <= a), L"<=演算子の結果が異常");
			Assert::IsTrue(b.ToString() == "5:20:34", L"ToStringの結果が異常");
		}

		TEST_METHOD(WeakPointerTest) {
			std::shared_ptr<Base> base0 = std::make_shared<Base>();
			std::shared_ptr<Child> child0 = std::make_shared<Child>();
			WeakPointer<Base> wb0 = base0;
			WeakPointer<Base> wb1 = child0;
//			WeakPointer<Child> cb0 = base0;
			WeakPointer<Child> cb1 = child0;
			Assert::IsTrue((bool)wb0, L"有効なWeakPointerのboolキャストの結果が異常");
			WeakPointer<Base> wbe;
			Assert::IsFalse((bool)wbe, L"空のWeakPointerのboolキャストの結果が異常");
			child0.reset(); //child0は解放された
			Assert::IsFalse((bool)wb1, L"解放されたWeakPointerのboolキャストの結果が異常");
			child0 = std::make_shared<Child>();
			cb1 = child0;
			wb1 = cb1;
			Assert::IsTrue((bool)wb1, L"アップキャストしたWeakPointerのboolキャストの結果が異常");
			child0.reset();
			base0.reset();
			base0 = std::make_shared<Child>();
			wb0 = base0;
			cb1 = static_weak_pointer_cast<Child>(wb0);
			Assert::IsTrue((bool)cb1, L"静的ダウンキャストしたWeakPointerのboolキャストの結果が異常");
			cb1 = dynamic_weak_pointer_cast<Child>(wb0);
			Assert::IsTrue((bool)cb1, L"動的ダウンキャストしたWeakPointerのboolキャストの結果が異常");
			try {
				cb1->x;
			}
			catch (NullWeakPointerException&) {
				Assert::Fail(L"有効なWeakPointerへのアクセスに失敗");
			}
			child0.reset();
			base0.reset();
			try {
				cb1->x;
				Assert::Fail(L"無効なWeakPointerへのアクセスの検出に失敗");
			}
			catch (NullWeakPointerException&) {
			}
		}
	};
}