#include "boost/filesystem.hpp"

#include "PlanetaEngine.h"

#include "StandardResourceManager.h"
#include "StandardLogManager.h"
#include "StandardSceneManager.h"
#include "StandardInputManager.h"
#include "StandardPerformanceManager.h"
#include "StandardRenderingManager.h"
#include "StandardSoundManager.h"
#include "StandardSaveManager.h"
#include "StandardDebugManager.h"
#include "StandardConfigManager.h"

#include "SystemVariables.h"

namespace planeta {
	void Game::SetStandardManagers() {
		using namespace private_;
		SetResourceManager(std::make_shared<StandardResourceManager>());
		auto log_mgr = std::make_shared<StandardLogManager>();
		//�J�����[�h�Ȃ�R���\�[���E�C���h�E���쐬���A�����Ƀ��O���o��
		if (system_variables::DevelopmentMode) {
			//�R���\�[���o�͂�L����
			log_mgr->ValidateConsoleOutPut();
		}
		//���O�f�B���N�g�����Ȃ���������
		if (!boost::filesystem::exists(system_variables::file_system::LogDirectory)) {
			boost::filesystem::create_directory(system_variables::file_system::LogDirectory);
		}
		//���O�o�̓t�@�C����L���ɂ���
		log_mgr->ValidateFileOutPut(system_variables::file_system::LogDirectory + "\\" + system_variables::file_system::LogOutPutFileName);
		SetLogManager(log_mgr);
		SetSceneManager(std::make_shared<private_::StandardSceneManager>());
		SetInputManager(std::make_shared<private_::StandardInputManager>());
		SetPerformanceManager(std::make_shared<private_::StandardPerformanceManager>());
		SetRenderingManager(std::make_shared<private_::StandardRenderingManager>());
		SetSoundManager(std::make_shared<private_::StandardSoundManager>());
		SetSaveManager(std::make_shared<private_::StandardSaveManager>());
		SetDebugManager(std::make_shared<private_::StandardDebugManager>());
		SetConfigManager(std::make_shared<private_::StandardConfigManager>());
	}
}