#pragma once

#include <memory>
#include <unordered_map>
#include<functional>
#include "Object.h"
#include "ParameterHolder.h"
#include "ISceneManagerAccessor.h"

namespace planeta_engine{
	class SceneSetUpper;
	class CollisionGroupMatrix;
	namespace core {
		class KeyInputManager;
		class Scene;
		/*�V�[���Ǘ��N���X
		�񓯊������͖����������A���������ꍇ�ɕύX�����Ȃ��Ȃ�悤�A�g�����͔񓯊�����������ꍇ�Ɠ����B
		*/
		class SceneManager : public core::Object,public ISceneManagerAccessor{
		public:
			explicit SceneManager();
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
			bool TransitionScene(const util::ParameterHolder& transition_parameters)override;
			/*�V�[����ǂݍ���őJ�ڂ���*/
			bool LoadAndTransitionScene(const std::string& scene_id, const util::ParameterHolder& transition_parameters = util::ParameterHolder())override {
				return LoadNextScene(scene_id) && TransitionScene(transition_parameters);
			}
			/*�I��*/
			void QuitScene()override { _request = _Request::Quit; }
			/*�G���[����*/
			void ErrorOccured()override { _request = _Request::Error; }

			//////////�V�X�e���֐�//////////
			/*�}�l�[�W���̏�����*/
			bool Initialize();
			/*�}�l�[�W���̏I������*/
			bool Finalize();
			/*�V�[���̏�(�p���A�I���A�G���[����)*/
			enum class SceneStatus_{ Continue, Quit, Error };
			/*�V�[������*/
			SceneStatus_ Process_();
			/*�Փ˃O���[�v�}�b�v��o�^*/
			void SetCollisionGroupMatrix_(std::shared_ptr<CollisionGroupMatrix>&& cg_matrix);
		private:
			/*�V�[���i�s�t���O*/
			bool _scene_progress_flag;
			/*���݂̃V�[��*/
			std::shared_ptr<Scene> _current_scene;
			/*���̃V�[��*/
			std::shared_ptr<Scene> _next_scene;
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
			util::ParameterHolder _transition_parameters;
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
			/*�Փ˃O���[�v�}�b�v*/
			std::shared_ptr<CollisionGroupMatrix> collision_group_matrix_;

			/*�J�ڏ���*/
			void _transition_proc();
			/*���݂̃V�[�����I��*/
			util::ParameterHolder _end_current_scene();
			/*�G���[�V�[���֑J��*/
			void _transition_to_error_scene();
			/*�V�[���̏�����*/
			bool InitializeScene_(Scene& scene,SceneSetUpper& setupper, const util::ParameterHolder& init_param);
			/*�V�[���̏I������*/
			util::ParameterHolder FinalizeScene_(core::Scene& scene, SceneSetUpper& setupper, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters);

		};
	}
}