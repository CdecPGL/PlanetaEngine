#include <cassert>

#include "DxLib.h"

#include "PlanetaEngine.h"
#include "InitEnd.h"
#include "SystemLog.h"
#include "SystemCounter.h"
#include "Game.h"
#include "DrawManager.h"

namespace planeta_engine {
	using namespace core;
	PlanetaEngine::PlanetaEngine() :is_initialized_(false) {

	}

	PlanetaEngine::~PlanetaEngine() {
		assert(!is_initialized_);
	}

	bool PlanetaEngine::Initialize() {
		if (is_initialized_) { assert(false); return false; }
		assert(game_ != nullptr);
		init_end::CreateSystemDirectory(); //�V�X�e���f�B���N�g���쐬
		if (InitializeDebugSystem() //�f�o�b�O�V�X�e��������
			&& init_end::LoadConfigData() //�ݒ�f�[�^�ǂݍ���
			&& InitializeEngine() //�G���W��������
			&& game_->Initialize()) //�Q�[��������
		{
			debug::SystemLog::instance().LogMessage("PlanetaEngine�����������܂����B", __FUNCTION__);
			is_initialized_ = true;
			return true;
		} else {
			debug::SystemLog::instance().LogError("PlanetaEngine�̏������Ɏ��s���܂����B", __FUNCTION__);
			is_initialized_ = true;
			Finalize();
			return false;
		}
	}

	bool PlanetaEngine::Finalize() {
		if (!is_initialized_) { assert(false); return false; }
		assert(game_ != nullptr);
		game_->Finalize();
		FinalzieEngine();
		debug::SystemLog::instance().LogMessage("PlanetaEngine���I�����܂����B", __FUNCTION__);
		FinalizeDebugSystem();
		is_initialized_ = false;
		return true;
	}

	PlanetaEngine::Status PlanetaEngine::Update() {
		if (ProcessMessage() < 0) { return Status::Quit; } //DX���C�u�����̍X�V����

		Status status;

		core::Game::Status gstatue = game_->Update();

		switch (gstatue) {
		case planeta_engine::core::Game::Status::Continue:status = Status::Continue; break;
		case planeta_engine::core::Game::Status::Quit:status = Status::Quit; break;
		case planeta_engine::core::Game::Status::Error:status = Status::Error; break;
		default:status = Status::Error; break;
		}

		debug::SystemCounter::instance().IncrementFrameCount();
		if (status == Status::Continue) {
			if (DrawManager::instance().Update() == false) { status = Status::Error; }
		}
		return status;
	}

	bool PlanetaEngine::InitializeEngine() {
		using namespace planeta_engine::core::init_end;

		if (InitDxLibrary() //DX���C�u�����̏�����
			&& SetUpSingletonManagers() //�V���O���g���}�l�[�W���̃Z�b�g�A�b�v
			&& InitializeSingletonManagers() //�V���O���g���}�l�[�W���̏�����
			) {
			debug::SystemLog::instance().LogMessage("PlanetaEngine�{�̂����������܂����B", __FUNCTION__);
			return true;
		} else {
			debug::SystemLog::instance().LogError("PlanetaEngine�{�̂̏������Ɏ��s���܂����B", __FUNCTION__);
			return false;
		}
	}

	void PlanetaEngine::FinalzieEngine() {
		using namespace planeta_engine::core::init_end;
		FinalizeSingletonManagers(); //�V���O���g���}�l�[�W���̏I������
		EndDxLibrary(); //DX���C�u�����̏I������
		debug::SystemLog::instance().LogMessage("PlanetaEngine�{�̂��I�����܂����B", __FUNCTION__);
	}

	bool PlanetaEngine::InitializeDebugSystem() {
		return init_end::InitializeDebugSystem();
	}

	void PlanetaEngine::FinalizeDebugSystem() {
		init_end::FinalizeDebugStstem();
	}
}