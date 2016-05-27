#include <cassert>
#include <functional>
#include <list>

#include "DxLib.h"

#include "InitFunctions.h"
#include "ProgramDefinitionData.h"

#include "PlanetaEngine.h"

#include "SystemLog.h"
#include "SystemTimer.h"
#include "RenderManager.h"
#include "DebugManager.h"
#include "FileSystemManager.h"
#include "KeyInputManager.h"
#include "ResourceManager.h"
#include "SaveDataManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

namespace planeta_engine {
	using namespace core;
	class PlanetaEngine::Impl_ {
	private:
		std::list<std::function<void()>> finalize_handls_;
		std::unique_ptr<SceneManager> scene_manager_;
	public:
		Impl_() :scene_manager_(std::make_unique<SceneManager>()) {}
		//�G���W���̏�����
		bool InitializeSubSystems() {
			//////////////////////////////////////////////////////////////////////////
			//�V�X�e���^�C�}�[�̏�����
			//////////////////////////////////////////////////////////////////////////
			if (debug::SystemTimer::instance().Initialize()) { finalize_handls_.push_front([] {debug::SystemTimer::instance().Finalize(); }); }
			else { assert(false); return false; }
			//////////////////////////////////////////////////////////////////////////
			//�V�X�e�����O�̏�����
			//////////////////////////////////////////////////////////////////////////
			{
				auto ret = init_funcs::InitializeLogSystem();
				if (std::get<0>(ret) == false) { return false; } 
				else { finalize_handls_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//�t�@�C���V�X�e���̏�����
			//////////////////////////////////////////////////////////////////////////
			FileSystemManager& flm = FileSystemManager::instance();
			//���\�[�X�p�t�@�C���A�N�Z�T�ݒ�
			auto resource_file_accesor = init_funcs::CreateFileAccessor(init_funcs::FileAccessorKind::Resource);
			//SaveData�p�t�@�C���A�N�Z�T�ݒ�
			auto savedata_dir_accesor = init_funcs::CreateFileAccessor(init_funcs::FileAccessorKind::SaveData);
			//system�p�t�@�C���A�N�Z�T�ݒ�
			auto system_dir_accesor = init_funcs::CreateFileAccessor(init_funcs::FileAccessorKind::System);
			//config�p�t�@�C���A�N�Z�T�ݒ�
			auto config_dir_accesor = init_funcs::CreateFileAccessor(init_funcs::FileAccessorKind::Config);
			if(flm.Initialize()){ finalize_handls_.push_front([] {FileSystemManager::instance().Finalize(); }); }
			else { PE_LOG_FATAL("�t�@�C���V�X�e���̏������Ɏ��s���܂����B"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//�G���W���ݒ�̓ǂݍ���
			//////////////////////////////////////////////////////////////////////////
			if (!init_funcs::LoadEngineConfig(system_dir_accesor)) { return false; }
			//////////////////////////////////////////////////////////////////////////
			//���\�[�X�V�X�e���̏�����
			//////////////////////////////////////////////////////////////////////////
			{
				auto ret = init_funcs::InitializeResourceSystem(resource_file_accesor);
				if (std::get<0>(ret) == false) { return false; }
				else { finalize_handls_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//�Z�[�u�f�[�^�V�X�e���̏�����
			//////////////////////////////////////////////////////////////////////////
			SaveDataManager::instance().SetFileAccessor_(savedata_dir_accesor);
			if(SaveDataManager::instance().Initialize()){ finalize_handls_.push_front([] {SaveDataManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("�Z�[�u�f�[�^�V�X�e���̏������Ɏ��s���܂����B"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//DX���C�u�����̏�����
			//////////////////////////////////////////////////////////////////////////
			{
				auto ret = init_funcs::InitializeDxLib();
				if (std::get<0>(ret) == false) { return false; } 
				else { finalize_handls_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//�`��V�X�e���̏�����
			//////////////////////////////////////////////////////////////////////////
			if(core::RenderManager::instance().Initialize()){ finalize_handls_.push_front([] {core::RenderManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("�`��V�X�e���̏������Ɏ��s���܂����B"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//�T�E���h�V�X�e���̏�����
			//////////////////////////////////////////////////////////////////////////
			if(core::SoundManager::instance().Initialize()){ finalize_handls_.push_front([] {core::SoundManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("�T�E���h�V�X�e���̏������Ɏ��s���܂����B"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//���̓V�X�e���̏�����
			//////////////////////////////////////////////////////////////////////////
			//�L�[�R���t�B�O�f�[�^�̃Z�b�g�\��
			if(core::KeyInputManager::instance().Initialize()){ finalize_handls_.push_front([] {core::KeyInputManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("���̓V�X�e���̏������Ɏ��s���܂����B"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//�f�o�b�O�V�X�e���̏�����
			//////////////////////////////////////////////////////////////////////////
			if(debug::DebugManager::instance().Initialize()){ finalize_handls_.push_front([] {debug::DebugManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("�f�o�b�O�V�X�e���̏������Ɏ��s���܂����B"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//�v���O�����p��`�̓ǂݍ���
			//////////////////////////////////////////////////////////////////////////
			core::ProgramDefinitionData pdd;
			if (!init_funcs::LoadProgramDefinition(system_dir_accesor, &pdd)) {
				PE_LOG_ERROR("�v���O�����p��`�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//�V�[���V�X�e���̏�����
			//////////////////////////////////////////////////////////////////////////
			//�Փ˃}�g���b�N�X�̐ݒ�
			scene_manager_->SetCollisionGroupMatrix_(std::make_shared<CollisionGroupMatrix>(std::move(pdd.collision_group_matrix)));
			if (scene_manager_->Initialize()) { finalize_handls_.push_front([&srn_mgr = *scene_manager_] {srn_mgr.Finalize(); }); }
			else { PE_LOG_FATAL("�V�[���V�X�e���̏������Ɏ��s���܂����B"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//�Q�[���̊J�n����
			//////////////////////////////////////////////////////////////////////////
			scene_manager_->LoadAndTransitionScene(pdd.startup_scene_id); //�X�^�[�g�A�b�v�V�[���̊J�n�˗�

			return true;

		}
		//�G���W���̏I������
		void FinalizeSubSystems() {
			for (auto&& fh : finalize_handls_) {
				fh();
			}
		}
		//�G���W���̍X�V
		GameStatus UpdateSubSystems() {
			if (ProcessMessage() < 0) { return GameStatus::Quit; } //DX���C�u�����̍X�V
			KeyInputManager::instance().Update(); //�L�[���͂̍X�V
			auto sst = scene_manager_->Process_(); //�V�[���̍X�V
			RenderManager::instance().Update(); //�`��V�X�e���̍X�V
			SoundManager::instance().Update(); //�T�E���h�V�X�e���̍X�V
			debug::SystemTimer::instance().IncrementFrameCount(); //�t���[���J�E���g�̃C���N�������g
			
			switch (sst) {
			case planeta_engine::core::SceneManager::SceneStatus_::Continue:
				return GameStatus::Continue;
			case planeta_engine::core::SceneManager::SceneStatus_::Quit:
				return GameStatus::Quit;
			case planeta_engine::core::SceneManager::SceneStatus_::Error:
				return GameStatus::Error;
			default:
				assert(false);
				return GameStatus::Error;
			}
		}
	};

	PlanetaEngine::PlanetaEngine() :is_initialized_(false), impl_(std::make_unique<Impl_>()) {

	}

	PlanetaEngine::~PlanetaEngine() {
		assert(!is_initialized_);
	}

	bool PlanetaEngine::Initialize() {
		if (is_initialized_) { assert(false); return false; }
		if (impl_->InitializeSubSystems()) {
			PE_LOG_MESSAGE("PlanetaEngine������ɏ���������܂����B");
			is_initialized_ = true;
			return true;
		} else {
			PE_LOG_FATAL("PlanetaEngine�̏������Ɏ��s���܂����B");
			impl_->FinalizeSubSystems();
			return false;
		}
	}

	void PlanetaEngine::Finalize() {
		if (!is_initialized_) { assert(false); return; }
		impl_->FinalizeSubSystems();
		PE_LOG_MESSAGE("PlanetaEngine���I�����܂����B");
	}

	GameStatus PlanetaEngine::Update() {
		return impl_->UpdateSubSystems();
	}
}