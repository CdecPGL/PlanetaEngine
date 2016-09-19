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
			Assert::IsTrue(a == a, L"==���Z�q�̌��ʂ��ُ�");
			Assert::IsFalse(a != a, L"!=���Z�q�̌��ʂ��ُ�");
			Assert::IsTrue(a + b == Time(15, 30, 44), L"���Ԃ̉��Z���ʂ��ُ�");
			Assert::IsTrue(a - b == Time(4, 49, 36), L"���Ԃ̌��Z���ʂ��ُ�");
			Assert::IsTrue(b*3 == Time(16, 1, 42), L"���Ԃ̐����{���ʂ��ُ�");
			Assert::IsTrue((a > b) && !(a > a) , L">���Z�q�̌��ʂ��ُ�");
			Assert::IsTrue(!(a < b) && !(a < a), L"<���Z�q�̌��ʂ��ُ�");
			Assert::IsTrue((a >= b) && (a >= a), L">=���Z�q�̌��ʂ��ُ�");
			Assert::IsTrue(!(a <= b) && (a <= a), L"<=���Z�q�̌��ʂ��ُ�");
			Assert::IsTrue(b.ToString() == "5:20:34", L"ToString�̌��ʂ��ُ�");
		}

		TEST_METHOD(WeakPointerTest) {
			std::shared_ptr<Base> base0 = std::make_shared<Base>();
			std::shared_ptr<Child> child0 = std::make_shared<Child>();
			WeakPointer<Base> wb0 = base0;
			WeakPointer<Base> wb1 = child0;
//			WeakPointer<Child> cb0 = base0;
			WeakPointer<Child> cb1 = child0;
			Assert::IsTrue((bool)wb0, L"�L����WeakPointer��bool�L���X�g�̌��ʂ��ُ�");
			WeakPointer<Base> wbe;
			Assert::IsFalse((bool)wbe, L"���WeakPointer��bool�L���X�g�̌��ʂ��ُ�");
			child0.reset(); //child0�͉�����ꂽ
			Assert::IsFalse((bool)wb1, L"������ꂽWeakPointer��bool�L���X�g�̌��ʂ��ُ�");
			child0 = std::make_shared<Child>();
			cb1 = child0;
			wb1 = cb1;
			Assert::IsTrue((bool)wb1, L"�A�b�v�L���X�g����WeakPointer��bool�L���X�g�̌��ʂ��ُ�");
			child0.reset();
			base0.reset();
			base0 = std::make_shared<Child>();
			wb0 = base0;
			cb1 = static_weak_pointer_cast<Child>(wb0);
			Assert::IsTrue((bool)cb1, L"�ÓI�_�E���L���X�g����WeakPointer��bool�L���X�g�̌��ʂ��ُ�");
			cb1 = dynamic_weak_pointer_cast<Child>(wb0);
			Assert::IsTrue((bool)cb1, L"���I�_�E���L���X�g����WeakPointer��bool�L���X�g�̌��ʂ��ُ�");
			try {
				cb1->x;
			}
			catch (NullWeakPointerException&) {
				Assert::Fail(L"�L����WeakPointer�ւ̃A�N�Z�X�Ɏ��s");
			}
			child0.reset();
			base0.reset();
			try {
				cb1->x;
				Assert::Fail(L"������WeakPointer�ւ̃A�N�Z�X�̌��o�Ɏ��s");
			}
			catch (NullWeakPointerException&) {
			}
		}
	};
}