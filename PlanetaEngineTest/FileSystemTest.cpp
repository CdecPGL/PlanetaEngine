#include "stdafx.h"
#include "CppUnitTest.h"
#include "SystemCounter.h"
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
	using namespace planeta_engine::file_system;
	TEST_CLASS(FileSystemTest)
	{
	public:
		FileSystemTest() {
			using namespace planeta_engine::debug;
			using namespace planeta_engine::file_system;
			SystemCounter::Instantiate();
			SystemLog::Instantiate();
			FileSystemManager::Instantiate();
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
			FileSystemManager::Dispose();
			SystemLog::Dispose();
			SystemCounter::Dispose();
		}

		TEST_METHOD(FileLoadManagerTest)
		{
			FileSystemManager& flm = FileSystemManager::instance();
			
		}
	};
}