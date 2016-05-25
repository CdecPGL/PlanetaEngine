#include "stdafx.h"
#include "CppUnitTest.h"
#include "SystemTimer.h"
#include "SystemLog.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlanetaEngineTest
{
	using namespace planeta_engine::debug;
	TEST_CLASS(DebugSystemTest)
	{
	public:

		TEST_METHOD(SystemCounterTest)
		{
			SystemCounter& sc = SystemCounter::instance();
			Assert::AreEqual(0u, sc.GetCurrentFrameCount(), L"初期フレームカウントが異常");
			for (int i = 0; i < 123; ++i) { sc.IncrementFrameCount(); }
			Assert::AreEqual(123u, sc.GetCurrentFrameCount(), L"加算後フレームカウントが異常");
		}

		TEST_METHOD(SystemLogTest) {
			SystemLog& sl = SystemLog::instance();
			std::stringstream test_stream0, test_stream1;
			sl.AddLogOutStream(test_stream0);
			sl.LogMessage("てすとまっせーじ","test");

			sl.AddLogOutStream(test_stream1);
			sl.LogWarning("てすと警告", "test1");
			sl.LogError("てすとエラー", "test1");
		}
	};
}