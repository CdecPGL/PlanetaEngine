#pragma once

#include <memory>
#include <unordered_map>
#include<functional>
#include "Object.h"
#include "ParameterHolder.h"
#include "ISceneManagerAccessor.h"
#include "SharedPointerInstance.h"

namespace planeta_engine{
	namespace core {
		class KeyInputManager;
		class Scene;
		class SceneSetUpper;
		class IGameAccessor;
		/*�V�[���Ǘ��N���X
		�񓯊������͖����������A���������ꍇ�ɕύX�����Ȃ��Ȃ�悤�A�g�����͔񓯊�����������ꍇ�Ɠ����B
		*/
		class SceneManager : public core::Object,public ISceneManagerAccessor{
		public:
			explicit SceneManager(IGameAccessor& engine);
			SceneManager(const SceneManager&) = delete;
			~SceneManager() = default;
			//////////���[�U�֐�//////////
			/*���V�[����ǂݍ���(�J�ڂ͂��Ȃ�)*/
			bool LoadNextScene(const std::string& scene_id)override;
			/*�ǂݍ��ݐi�s�x�擾(0.0-1.0�B���ǂݍ��ݎ��ɂ�0.0)*/
			double GetLoadProgress()const override { return _is_loading ? _load_progress : _is_next_scene_loaded ? 1.0f : 0.0f; }
			/*�ǂݍ��ݒ���*/
			bool IsLoading()const override { return _is_loading; }
			/*�J�ڒ���*/
			bool IsTransitioning()const override { return _is_transitioning; }
			/*�J�ډ\��*/
			bool IsTransitionable()const override { return (_is_next_scene_loaded || _is_loading) && !_is_transitioning; }
			/*���̃V�[��ID���擾*/
			const std::string GetNextSceneID()const override { return IsTransitionable() ? _next_scene_id : "\0"; }
			/*���݂̃V�[��ID���擾*/
			const std::string GetCurrentSceneID()const override { return _current_scene_id; }
			/*�V�[���̑J��(�ǂݍ��݂��K�v�B�ǂݍ��ݒ���������ǂݍ��݂J��)*/
			bool TransitionScene(const utility::ParameterHolder& transition_parameters)override;
			/*�V�[����ǂݍ���őJ�ڂ���*/
			bool LoadAndTransitionScene(const std::string& scene_id, const utility::ParameterHolder& transition_parameters = utility::ParameterHolder())override {
				return LoadNextScene(scene_id) && TransitionScene(transition_parameters);
			}

			//////////�V�X�e���֐�//////////
			/*�}�l�[�W���̏�����*/
			bool Initialize();
			/*�}�l�[�W���̏I������*/
			bool Finalize();
			/*�V�[���̏�(�p���A�I���A�G���[����)*/
			enum class SceneStatus{ Continue, Quit, Error };
			/*�V�[������*/
			SceneStatus Process();

			/*�V�[���Z�b�^�[��ǉ�����(�V�[����`�N���G�[�^���쐬���āA�����ێ�����)*/
			template<class C>
			void AddSceneSetUpper(const std::string& scene_name){
				static_assert(std::is_base_of<SceneSetUpper, C>::value == true, "C is not derived SceneSetUpper.");
				_scene_setupper_creaters.emplace(scene_name, []()->std::shared_ptr<SceneSetUpper> {return std::make_shared<C>(); });
			}
			/*�V�[���Z�b�^�[�N���G�[�^��ǉ�����(�O���t�@�C���ɂ��V�[���Z�b�g�A�b�p�[�쐬�Ɏg�p����\��)*/
			void AddSceneSetUpper(const std::string& scene_name,const std::function<std::shared_ptr<SceneSetUpper>()>& func) {
				_scene_setupper_creaters.emplace(scene_name, func);
			}
			/*�I��*/
			void QuitScene()override { _request = _Request::Quit; }
			/*�G���[����*/
			void ErrorOccured()override { _request = _Request::Error; }
		private:
			/*�V�[���i�s�t���O*/
			bool _scene_progress_flag;
			/*���݂̃V�[��*/
			std::shared_ptr<Scene> _current_scene;
			/*���̃V�[��*/
			std::shared_ptr<Scene> _next_scene;
			/*�V�[���ݒ�N���X�N���G�[�^*/
			std::unordered_map<std::string, std::function<std::shared_ptr<SceneSetUpper>()>> _scene_setupper_creaters;
			/*�V�[���ݒ�N���X�쐬*/
			std::shared_ptr<SceneSetUpper> _CreateSceneSetUpper(const std::string& scene_name);
			/*���݂̃V�[���Z�b�g�N���X*/
			std::shared_ptr<SceneSetUpper> _current_scene_setupper;
			/*���̃V�[���Z�b�g�N���X*/
			std::shared_ptr<SceneSetUpper> _next_scene_setupper;
			/*���N�G�X�g*/
			enum class _Request { None, Transition, Quit, Error };
			_Request _request;
			/*���̃V�[��ID*/
			std::string _next_scene_id;
			/*���̃V�[���̏������p�����[�^���X�g*/
			utility::ParameterHolder _transition_parameters;
			/*���݂̃V�[��ID*/
			std::string _current_scene_id;
			/*�J�ڒ���*/
			bool _is_transitioning;
			/*�ǂݍ��ݒ���*/
			bool _is_loading;
			/*���̃V�[�����ǂݍ��܂�Ă��邩*/
			bool _is_next_scene_loaded;
			/*�ǂݍ��ݐi�s�x*/
			double _load_progress;
			/*�G���W���ւ̎Q��*/
			IGameAccessor& _game;

			/*�J�ڏ���*/
			void _transition_proc();
			/*���݂̃V�[�����I��*/
			utility::ParameterHolder _end_current_scene();
			/*�G���[�V�[���֑J��*/
			void _transition_to_error_scene();

		};
	}
}