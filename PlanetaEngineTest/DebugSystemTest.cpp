#include "stdafx.h"
#include "CppUnitTest.h"
#include "SystemTimer.h"
#include "SystemLog.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlanetaEngineTest
{
	using namespace planeta::debug;
	TEST_CLASS(DebugSystemTest)
	{
	public:

		TEST_METHOD(SystemCounterTest)
		{
			SystemTimer& sc = SystemTimer::instance();
			Assert::AreEqual(0u, sc.GetCurrentFrameCount(), L"�����t���[���J�E���g���ُ�");
			for (int i = 0; i < 123; ++i) { sc.IncrementFrameCount(); }
			Assert::AreEqual(123u, sc.GetCurrentFrameCount(), L"���Z��t���[���J�E���g���ُ�");
		}

		TEST_METHOD(SystemLogTest) {
			SystemLog& sl = SystemLog::instance();
			std::stringstream test_stream0, test_stream1;
			sl.AddLogOutStream(test_stream0);
			//PE_LOG_MESSAGE("�Ă��Ƃ܂����[��");

			sl.AddLogOutStream(test_stream1);
			//PE_LOG_WARNING("�Ă��ƌx��");
			//PE_LOG_ERROR("�Ă��ƃG���[");
		}
	};
}