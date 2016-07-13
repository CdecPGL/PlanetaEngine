#include "SceneManager.h"
#include "Scene.h"
#include "SceneSetUpper.h"
#include "ResourceManager.h"
#include "SError.h"
#include "SEmpty.h"
#include "SystemLog.h"
#include "NullWeakPointerException.h"
#include "SceneSystemSetUpper.h"
#include "SystemVariables.h"

namespace planeta{
	namespace core{
		SceneManager::SceneManager() :_scene_progress_flag(true), _request(_Request::None), _is_transitioning(false), _is_loading(false), _is_next_scene_loaded(false), _load_progress(0.0) {

		}


		SceneManager::SceneStatus_ SceneManager::Process_()
		{
			try {
				switch (_request)
				{
				case planeta::core::SceneManager::_Request::Transition:
					_transition_proc();
				case planeta::core::SceneManager::_Request::None:
					if (_scene_progress_flag) { _current_scene->Update(); }
					return SceneStatus_::Continue;
				case planeta::core::SceneManager::_Request::Quit:
					return SceneStatus_::Quit;
				case planeta::core::SceneManager::_Request::Error:
					return SceneStatus_::Error;
				default:
					break;
				}
				return SceneStatus_::Error;
			}
			catch (util::NullWeakPointerException& e) {
				PE_LOG_ERROR("Scene::Update�Ŗ�����WeakPointer���Q�Ƃ���܂����B", e.what());
				return SceneStatus_::Error;
			}
		}

		bool SceneManager::LoadNextScene(const std::string& scene_name){
			if (IsLoading() || IsTransitioning()){ 
				PE_LOG_ERROR("�V�[���̓ǂݍ��݂Ɏ��s���܂����B�V�[���̓ǂݍ��ݒ��A�܂��͑J�ڒ��̂��ߐV���ɃV�[��(", scene_name, ")��ǂݍ��ނ��Ƃ͂ł��܂���B");
				return false;
			} //���łɓǂݍ��ݒ����A�J�ڒ��̂��߁A���炽�ɓǂݍ��݂ł��Ȃ�
			//�V�[����`�N���X�쐬
			_next_scene_setupper = _CreateSceneSetUpper(scene_name);
			if (_next_scene_setupper == nullptr) { 
				PE_LOG_ERROR("�V�[���̓ǂݍ��݂Ɏ��s���܂����B�w�肳�ꂽ�V�[��(", scene_name, ")�̃Z�b�g�A�b�p�[�����݂��܂���B");
				return false; 
			} //�V�[����`�N���X���쐬�ł��Ȃ�����
			//���݂̃V�[������J�ډ\�ȃV�[�����m�F(���݂̃V�[�����󂾂�����m�F���Ȃ�)
			if (_current_scene!=nullptr && _current_scene_setupper->CheckTransitionable(scene_name) == false) {
				PE_LOG_ERROR("�V�[���̓ǂݍ��݂Ɏ��s���܂����B�w�肳�ꂽ�V�[��(", scene_name, ")�͌��݂̃V�[������J�ډ\�ȃV�[���ł͂���܂���B");
				return false;
			}
			//���\�[�X�ǂݍ���
			if (core::ResourceManager::instance().PrepareResources({scene_name}) == false) {
				PE_LOG_WARNING("�w�肳�ꂽ�V�[��(", scene_name, ")�̃��\�[�X�����Ɏ��s���܂����B�ǂݍ��݂Ɏ��s�������A�Ώۂ̃��\�[�X�����݂��Ȃ��\��������܂��B");
				//return false;
			} //���\�[�X�̏������ł��Ȃ�����
			//��������
			_next_scene_id = scene_name;
			_is_next_scene_loaded = true;
			PE_LOG_MESSAGE("�w�肳�ꂽ�V�[��(", scene_name, ")��ǂݍ��݂܂����B");
			return true;
		}

		bool SceneManager::TransitionScene(const util::ParameterHolder& transition_parameters)
		{
			if (IsTransitionable()==false){
				PE_LOG_ERROR("�V�[���J�ڗ\��Ɏ��s���܂����B�J�ڒ��̂��߁A�V���ɑJ�ڏ������͂��߂邱�Ƃ͂ł��܂���B");
				return false;
			} //�J�ڏ������n�߂邱�Ƃ͂ł��Ȃ�
			//�V�[���J�ڗv��
			_request = _Request::Transition;
			_transition_parameters = transition_parameters;
			PE_LOG_MESSAGE("�V�[���J�ڗ\����s���܂����B");
			return true;
		}

		void SceneManager::_transition_proc()
		{
			//���݂̃V�[�����I��
			util::ParameterHolder next_scene_initialize_parameters = _end_current_scene();
			//�V�����V�[�����쐬
			std::shared_ptr<Scene> new_scene = std::make_shared<Scene>();
			//�V�����V�[����������
			if (!InitializeScene_(*new_scene,*_next_scene_setupper,next_scene_initialize_parameters)) {
				PE_LOG_ERROR("�V�[���J�ڂɎ��s���܂����B�V�����V�[��(", _next_scene_id, ")�̃Z�b�g�A�b�v�܂��͏������Ɏ��s���܂����B");
				_transition_to_error_scene();
				return;
			}
			_current_scene = std::move(new_scene);
			_current_scene_setupper = std::move(_next_scene_setupper);
			//���g�p���\�[�X���폜(�V�[���̍X�V�O�̂��߁A�����ō폜���Ă悢)
			core::ResourceManager::instance().UnloadUnusedResouces();
			//���N�G�X�g�Ə����󋵂����Z�b�g
			_request = _Request::None;
			_is_next_scene_loaded = false;
			PE_LOG_MESSAGE("�V�[��(", _next_scene_id, ")�ɑJ�ڂ��܂����B");
			_next_scene_id = "";
		}

		bool SceneManager::Initialize()
		{
			//��̃V�[�����Z�b�g
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<SEmpty>();
			std::shared_ptr<Scene> es = std::make_shared<Scene>();
			InitializeScene_(*es, *ecd, util::ParameterHolder());
			_current_scene = std::move(es);
			_current_scene_setupper = ecd;
			_request = _Request::None;
			_is_next_scene_loaded = false;

			return true;
		}

		bool SceneManager::Finalize()
		{
			//���݂̃V�[�����I��
			_end_current_scene();
			return true;
		}

		std::shared_ptr<SceneSetUpper> SceneManager::_CreateSceneSetUpper(const std::string& scene_name)
		{
			//�V�[�����Ƀv���t�B�b�N�X�������N���X���쐬�B
			auto setupper = Reflection::CreateObjectByID<SceneSetUpper>(system_variables::prefixes::Scene + scene_name);
			return setupper;
		}

		void SceneManager::_transition_to_error_scene()
		{
			PE_LOG_ERROR("�G���[�V�[���ɑJ�ڂ��܂��B");
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<SError>();
			std::shared_ptr<Scene> es = std::make_shared<Scene>();
			InitializeScene_(*es, *ecd, util::ParameterHolder());
			_current_scene = std::move(es);
			_current_scene_setupper = ecd;
			_request = _Request::None;
			_is_next_scene_loaded = false;
		}

		util::ParameterHolder SceneManager::_end_current_scene()
		{
			//���݂̃V�[�����I��(���݂̃V�[�����Ȃ��������̃p�����[�^���i�[)
			if (_current_scene) {
				return FinalizeScene_(*_current_scene, *_current_scene_setupper, _next_scene_id, _transition_parameters);
			}
			else { return util::ParameterHolder(); }
		}

		void SceneManager::SetCollisionGroupMatrix_(std::shared_ptr<CollisionGroupMatrix>&& cg_matrix) {
			collision_group_matrix_ = std::move(cg_matrix);
		}

		bool SceneManager::InitializeScene_(Scene& scene, SceneSetUpper& setupper, const util::ParameterHolder& init_param) {
			SceneSetUpProxy safs(scene);
			//�V�[���f�[�^�̏���
			scene.SetCollisionGroupMatrix(collision_group_matrix_);
			scene.PrepareSceneData(this);
			//�V�X�e���ݒ�(����v���Z�X�̍쐬��V�[���f�[�^�̍X�V)
			if (!core::SceneSystemSetUpper()(scene)) {
				PE_LOG_ERROR("�V�[���̃V�X�e���ݒ�Ɏ��s���܂����B");
				return false;
			}
			//�V�[���f�[�^�����W���[���ɓo�^
			scene.RegisterSceneDataToModules();
			//�ŗL�ݒ�
			if (!setupper.InitializeScene(scene, init_param)) {
				PE_LOG_ERROR("�V�[���̌ŗL�ݒ�Ɏ��s���܂����B");
				return false;
			}
			//������
			if (!scene.Initialize()) {
				PE_LOG_ERROR("�V�[���̏������Ɏ��s���܂����B");
				return false;
			}
			return true;
		}

		util::ParameterHolder SceneManager::FinalizeScene_(core::Scene& scene, SceneSetUpper& setupper, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters) {
			SceneSetUpProxy safs(scene);
			auto ret = setupper.TerminateScene(scene,next_scene_id, finalize_parameters); //�ŗL�I������
			scene.Finalize(); //�I������
			return ret;
		}

	}
}