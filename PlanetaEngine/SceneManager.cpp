#include "SceneManager.h"
#include "Scene.h"
#include "SceneSetUpper.h"
#include "ResourceManager.h"
#include "ErrorSceneDefinition.h"
#include "EmptySceneDefinition.h"
#include "SystemLog.h"
#include "NullWeakPointerException.h"

namespace planeta_engine{
	namespace core{
		SceneManager::SceneManager(IGameAccessor& engine) :_game(engine),_scene_progress_flag(true), _request(_Request::None), _is_transitioning(false), _is_loading(false), _is_next_scene_loaded(false), _load_progress(0.0) {

		}


		SceneManager::SceneStatus SceneManager::Process()
		{
			try {
				switch (_request)
				{
				case planeta_engine::core::SceneManager::_Request::Transition:
					_transition_proc();
				case planeta_engine::core::SceneManager::_Request::None:
					if (_scene_progress_flag) { _current_scene->Update(); }
					return SceneStatus::Continue;
				case planeta_engine::core::SceneManager::_Request::Quit:
					return SceneStatus::Quit;
				case planeta_engine::core::SceneManager::_Request::Error:
					return SceneStatus::Error;
				default:
					break;
				}
				return SceneStatus::Error;
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("Scene::Update�Ŗ�����WeakPointer���Q�Ƃ���܂����B") + e.what(), __FUNCTION__);
				return SceneStatus::Error;
			}
		}

		bool SceneManager::LoadNextScene(const std::string& scene_name){
			if (IsLoading() || IsTransitioning()){ 
				debug::SystemLog::instance().LogError(std::string("�V�[���̓ǂݍ��݂Ɏ��s���܂����B�V�[���̓ǂݍ��ݒ��A�܂��͑J�ڒ��̂��ߐV���ɃV�[��(") + scene_name + ")��ǂݍ��ނ��Ƃ͂ł��܂���B", __FUNCTION__);
				return false;
			} //���łɓǂݍ��ݒ����A�J�ڒ��̂��߁A���炽�ɓǂݍ��݂ł��Ȃ�
			//�V�[����`�N���X�쐬
			_next_scene_setupper = _CreateSceneSetUpper(scene_name);
			if (_next_scene_setupper == nullptr) { 
				debug::SystemLog::instance().LogError(std::string("�V�[���̓ǂݍ��݂Ɏ��s���܂����B�w�肳�ꂽ�V�[��(") + scene_name + ")�̃Z�b�g�A�b�p�[�����݂��܂���B", __FUNCTION__);
				return false; 
			} //�V�[����`�N���X���쐬�ł��Ȃ�����
			//���݂̃V�[������J�ډ\�ȃV�[�����m�F(���݂̃V�[�����󂾂�����m�F���Ȃ�)
			if (_current_scene!=nullptr && _current_scene_setupper->CheckTransitionable(scene_name) == false) {
				debug::SystemLog::instance().LogError(std::string("�V�[���̓ǂݍ��݂Ɏ��s���܂����B�w�肳�ꂽ�V�[��(") + scene_name + ")�͌��݂̃V�[������J�ډ\�ȃV�[���ł͂���܂���B", __FUNCTION__);
				return false;
			}
			//���\�[�X�ǂݍ���
			const std::vector<std::string>& ntags = _next_scene_setupper->GetUseTagGroups();
			if (core::ResourceManager::instance().PrepareResources(ntags) == false) {
				debug::SystemLog::instance().LogError(std::string("�V�[���̓ǂݍ��݂Ɏ��s���܂����B�w�肳�ꂽ�V�[��(") + scene_name + ")�̃��\�[�X�����Ɏ��s���܂����B", __FUNCTION__);
				return false; 
			} //���\�[�X�̏������ł��Ȃ�����
			//��������
			_next_scene_id = scene_name;
			_is_next_scene_loaded = true;
			debug::SystemLog::instance().LogMessage(std::string("�w�肳�ꂽ�V�[��(") + scene_name + ")��ǂݍ��݂܂����B", __FUNCTION__);
			return true;
		}

		bool SceneManager::TransitionScene(const utility::ParameterHolder& transition_parameters)
		{
			if (IsTransitionable()==false){
				debug::SystemLog::instance().LogError("�V�[���J�ڗ\��Ɏ��s���܂����B�J�ڒ��̂��߁A�V���ɑJ�ڏ������͂��߂邱�Ƃ͂ł��܂���B", __FUNCTION__);
				return false;
			} //�J�ڏ������n�߂邱�Ƃ͂ł��Ȃ�
			//�V�[���J�ڗv��
			_request = _Request::Transition;
			_transition_parameters = transition_parameters;
			debug::SystemLog::instance().LogMessage("�V�[���J�ڗ\����s���܂����B", __FUNCTION__);
			return true;
		}

		void SceneManager::_transition_proc()
		{
			//���݂̃V�[�����I��
			utility::ParameterHolder next_scene_initialize_parameters = _end_current_scene();
			//�V�����V�[�����쐬
			std::shared_ptr<Scene> new_scene = Scene::MakeShared(_game);
			//�V�����V�[����������
			if (!(_next_scene_setupper->InitializeScene(*new_scene, next_scene_initialize_parameters))) {
				debug::SystemLog::instance().LogError(std::string("�V�[���J�ڂɎ��s���܂����B�V�����V�[��(") + _next_scene_id + ")�̃Z�b�g�A�b�v�܂��͏������Ɏ��s���܂����B", __FUNCTION__);
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
			debug::SystemLog::instance().LogMessage(std::string("�V�[��(") + _next_scene_id + ")�ɑJ�ڂ��܂����B", __FUNCTION__);
			_next_scene_id = "";
		}

		bool SceneManager::Initialize()
		{
			//��̃V�[�����Z�b�g
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<EmptySceneDefinition>();
			std::shared_ptr<Scene> es = Scene::MakeShared(_game);
			ecd->InitializeScene(*es, utility::ParameterHolder());
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
			auto it = _scene_setupper_creaters.find(scene_name);
			if (it == _scene_setupper_creaters.end()) { return nullptr; }
			else {
				return (it->second)();
			}
		}

		void SceneManager::_transition_to_error_scene()
		{
			debug::SystemLog::instance().LogError("�G���[�V�[���ɑJ�ڂ��܂��B", __FUNCTION__);
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<ErrorSceneDefinition>();
			std::shared_ptr<Scene> es = Scene::MakeShared(_game);
			ecd->InitializeScene(*es, utility::ParameterHolder());
			_current_scene = std::move(es);
			_current_scene_setupper = ecd;
			_request = _Request::None;
			_is_next_scene_loaded = false;
		}

		utility::ParameterHolder SceneManager::_end_current_scene()
		{
			//���݂̃V�[�����I��(���݂̃V�[�����Ȃ��������̃p�����[�^���i�[)
			if (_current_scene) {
				return _current_scene_setupper->TerminateScene(*_current_scene, _next_scene_id, _transition_parameters);
			}
			else { return utility::ParameterHolder(); }
		}

	}
}