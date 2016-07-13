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
			Assert::AreEqual(0u, sc.GetCurrentFrameCount(), L"初期フレームカウントが異常");
			for (int i = 0; i < 123; ++i) { sc.IncrementFrameCount(); }
			Assert::AreEqual(123u, sc.GetCurrentFrameCount(), L"加算後フレームカウントが異常");
		}

		TEST_METHOD(SystemLogTest) {
			SystemLog& sl = SystemLog::instance();
			std::stringstream test_stream0, test_stream1;
			sl.AddLogOutStream(test_stream0);
			//PE_LOG_MESSAGE("てすとまっせーじ");

			sl.AddLogOutStream(test_stream1);
			//PE_LOG_WARNING("てすと警告");
			//PE_LOG_ERROR("てすとエラー");
		}
	};
}