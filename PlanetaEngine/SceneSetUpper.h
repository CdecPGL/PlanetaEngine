#pragma once
#include "Object.h"
#include <vector>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include "ParameterHolder.h"
namespace planeta_engine{
	class SceneAccessorForSetUp;
	namespace core{
		class Scene;
		/*�V�[���ݒ�N���X�B�I�u�W�F�N�g���p�������͖�����*/
		class SceneSetUpper : public core::Object{
		public:
			SceneSetUpper() = default;
			virtual ~SceneSetUpper() = default;
			/*�V�[��������������
			@param �ΏۃV�[���A�������p�����[�^���X�g
			*/
			bool InitializeScene(Scene& scene, const utility::ParameterHolder& initialize_parameters);
			/*�V�[�����I������
			@param �ΏۃV�[���A���̃V�[����ID�A�I�������p�����[�^���X�g
			@return ���̃V�[���̏������p�����[�^
			*/
			utility::ParameterHolder TerminateScene(Scene& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters);
			/*�g�p���\�[�X�̃^�O�O���[�v���X�g��Ԃ�*/
			virtual const std::vector<std::string> GetUseTagGroups()const = 0;
			/*�J�ډ\SceneID���X�g(����̏ꍇ�A�S�ẴV�[���ɑJ�ډ\�Ƃ݂Ȃ�)*/
			virtual const std::vector<std::string> GetTransitionableSceneID()const = 0;
			/*�w�肵���V�[���ɑJ�ډ\��*/
			bool CheckTransitionable(const std::string& scene_id) {
				auto l = GetTransitionableSceneID();
				return l.size() == 0 ||  std::find(l.begin(), l.end(), scene_id) != l.end();
			}
		private:
			/*�V�[���̐ݒ菈�����s��(�����I�u�W�F�N�g�AUI�A�v���Z�X�̒ǉ��A[�O�V�[������I�u�W�F�N�g�̈��p��]������)
			@param �ݒ肷��V�[���A�������p�����[�^���X�g
			*/
			virtual bool SetUpScene(SceneAccessorForSetUp& scene, const utility::ParameterHolder& initialize_parameters) = 0;
			/*�V�[���̏I���������s��(���V�[���̏������p�����[�^�����Ȃ�)
			@param �I����������V�[���A���̃V�[����ID�A�I�������p�����[�^���X�g
			@return ���̃V�[���̏������p�����[�^
			*/
			virtual utility::ParameterHolder FinalizeScene(SceneAccessorForSetUp& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) = 0;
		};
	}
}