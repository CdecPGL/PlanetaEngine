#include "PlanetaEngine.h"
#include "InitEnd.h"
#include "SceneManager.h"
#include "SystemCounter.h"
#include "ResourceManager.h"
#include "SystemLog.h"
#include "DxLib.h"
#include "DrawManager.h"
#include "NullWeakPointerException.h"
#include "KeyInputManager.h"
#include "Game.h"

//#include "GlobalNewDeleteDefinition.h"

namespace planeta_engine {
	namespace core {
		PlanetaEngine::PlanetaEngine() :_is_initialized(false),_game(std::make_unique<Game>())
		{

		}

		PlanetaEngine::~PlanetaEngine()
		{
			if (_is_initialized) { Finalize(); }
		}

		bool PlanetaEngine::Initialize()
		{
			using namespace planeta_engine::core::init_end;

//			memory_allocator.ReserveMemory(1024 * 1024 * 10);

			CreateSystemDirectory();
			if (!InitializeDebugSystem()) { //�f�o�b�O�V�X�e���̏�����
				return false;
			}
			if (InstantiateSingletonManagers() //�V���O���g���}�l�[�W���̃C���X�^���X��
				&&InitDxLibrary() //DX���C�u�����̏�����
				&& SetUpSingletonManagers() //�V���O���g���}�l�[�W���̃Z�b�g�A�b�v
				&& _InitializeGame() //�ŗL������
				&& InitializeSingletonManagers() //�V���O���g���}�l�[�W���̏�����
				&& _game->Initialize() //�Q�[���N���X�̏�����
				) {
				debug::SystemLog::instance().LogMessage("PlanetaEngine�̏������ɐ������܂����B", "PlanetaEngine::Initialize");
				_is_initialized = true;
			}
			else {
				debug::SystemLog::instance().LogError("PlanetaEngine�̏������Ɏ��s���܂����B", "PlanetaEngine::Initialize");
				_is_initialized = false;
				return false;
			}
			//�X�^�[�g�A�b�v�V�[���̊J�n
			if (_game->scene_manager().LoadAndTransitionScene(_StartUpSceneName())) {
				debug::SystemLog::instance().LogMessage(std::string("�X�^�[�g�A�b�v�V�[��(") + _StartUpSceneName() + ")���J�n���܂��B", "PlanetaEngine::Initialize");
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("�X�^�[�g�A�b�v�V�[��(")+_StartUpSceneName()+")�̊J�n�Ɏ��s���܂����B", "PlanetaEngine::Initialize");
				return false;
			}
		}

		void PlanetaEngine::Finalize()
		{
			using namespace planeta_engine::core::init_end;

			_game->Finalize(); //�Q�[���N���X�̏I������
			FinalizeSingletonManagers(); //�V���O���g���}�l�[�W���̏I������
			DisposeSingletonManagers(); //�V���O���g���}�l�[�W���̔j��
			EndDxLibrary(); //DX���C�u�����̏I������
			debug::SystemLog::instance().LogMessage("PlanetaEngine�̏I�����������s���܂����B", "PlanetaEngine::Finalize");
			FinalizeDebugStstem(); //�f�o�b�O�V�X�e���̏I������
			_is_initialized = false;
		}

		PlanetaEngine::Status PlanetaEngine::Update()
		{
			if (ProcessMessage() < 0) { return Status::Quit; } //DX���C�u�����̍X�V����
			
			Status status;

			Game::Status gstatue = _game->Update();

			switch (gstatue)
			{
			case planeta_engine::core::Game::Status::Continue:status = Status::Continue; break;
			case planeta_engine::core::Game::Status::Quit:status = Status::Quit; break;
			case planeta_engine::core::Game::Status::Error:status = Status::Error; break;
			default:status = Status::Error; break;
			}

			debug::SystemCounter::instance().IncrementFrameCount();
			if (status == Status::Continue) {
				if (DrawManager::instance().Update()==false) { status = Status::Error; }
			}
			return status;
		}

		IGameSetUp& PlanetaEngine::game_setup_access()
		{
			return *_game;
		}

	}
}