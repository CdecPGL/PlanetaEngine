#include "stdafx.h"
#include "CppUnitTest.h"
#include "SystemTimer.h"
#include "SystemLog.h"
#include "FileSystemManager.h"
#include "ArchiveManipulator.h"
#include "NormalFolderManipulator.h"
#include "FileAccessMode.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace {
	const std::string kTestFolder("test_folder");
	const unsigned int kTestArchivePass(12345);
	const std::string kTestArchive("test_archive");
}

namespace PlanetaEngineTest
{
	using namespace planeta_engine;
	TEST_CLASS(FileSystemTest)
	{
	public:
		FileSystemTest() {
			using namespace planeta_engine::debug;
			SystemCounter::instance().Initialize();
			SystemLog::instance().Initialize();

			FileSystemManager::instance().CreateFileAccessor("archive_test", std::make_shared<ArchiveManipulator>(kTestArchive, kTestArchivePass), AccessMode::ReadWrite);
			FileSystemManager::instance().CreateFileAccessor("normal_folder_test", std::make_shared<NormalFolderManipulator>(kTestArchive, kTestArchivePass), AccessMode::ReadWrite);
			FileSystemManager::instance().Initialize();
		}

		~FileSystemTest() {
			using namespace planeta_engine::debug;
			FileSystemManager::instance().Finalize();
			SystemLog::instance().Finalize();
			SystemCounter::instance().Finalize();
		}

		TEST_METHOD(FileLoadManagerTest)
		{
			FileSystemManager& flm = FileSystemManager::instance();
			
		}
	};
}