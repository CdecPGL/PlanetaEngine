#include "stdafx.h"
#include "CppUnitTest.h"
#include "SystemCounter.h"
#include "SystemLog.h"
#include "FileLoadManager.h"
#include "ArchiveManipulator.h"
#include "NormalFolderLoader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace {
	const std::string kTestFolder("test_folder");
	const unsigned int kTestArchivePass(12345);
	const std::string kTestArchive("test_archive");
}

namespace PlanetaEngineTest
{
	using namespace planeta_engine::file_system;
	TEST_CLASS(FileSystemTest)
	{
	public:
		FileSystemTest() {
			using namespace planeta_engine::debug;
			SystemCounter::Instantiate();
			SystemLog::Instantiate();
			FileLoadManager::Instantiate();
			SystemCounter::instance().Initialize();
			SystemLog::instance().Initialize();

			FileLoadManager::instance().PushLoader(std::make_shared<ArchiveManipulator>(kTestArchive, kTestArchivePass));
			FileLoadManager::instance().PushLoader(std::make_shared<NormalFolderManipulator>(kTestFolder));
			FileLoadManager::instance().Initialize();
		}

		~FileSystemTest() {
			using namespace planeta_engine::debug;
			FileLoadManager::instance().Finalize();
			SystemLog::instance().Finalize();
			SystemCounter::instance().Finalize();
			FileLoadManager::Dispose();
			SystemLog::Dispose();
			SystemCounter::Dispose();
		}

		TEST_METHOD(FileLoadManagerTest)
		{
			FileLoadManager& flm = FileLoadManager::instance();
			
		}
	};
}