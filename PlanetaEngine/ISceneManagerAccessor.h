#pragma once

#include <string>
#include "ParameterHolder.h"

namespace planeta_engine {
	namespace core {
		class ISceneManagerAccessor {
		public: 
			virtual ~ISceneManagerAccessor() = 0 {};
			/*���V�[����ǂݍ���(�J�ڂ͂��Ȃ�)*/
			virtual bool LoadNextScene(const std::string& scene_id) = 0;
			/*�ǂݍ��ݐi�s�x�擾(0.0-1.0�B���ǂݍ��ݎ��ɂ�0.0)*/
			virtual double GetLoadProgress()const = 0;
			/*�ǂݍ��ݒ���*/
			virtual bool IsLoading()const = 0;
			/*�J�ڒ���*/
			virtual bool IsTransitioning()const = 0;
			/*�J�ډ\��*/
			virtual bool IsTransitionable()const = 0;
			/*���̃V�[��ID���擾*/
			virtual const std::string GetNextSceneID()const = 0;
			/*���݂̃V�[��ID���擾*/
			virtual const std::string GetCurrentSceneID()const = 0;
			/*�V�[���̑J��(�ǂݍ��݂��K�v�B�ǂݍ��ݒ���������ǂݍ��݂J��)*/
			virtual bool TransitionScene(const utility::ParameterHolder& transition_parameters) = 0;
			/*�V�[����ǂݍ���őJ�ڂ���*/
			virtual bool LoadAndTransitionScene(const std::string& scene_id, const utility::ParameterHolder& transition_parameters = utility::ParameterHolder()) = 0;
			/*�I��*/
			virtual void QuitScene() = 0;
			/*�G���[����*/
			virtual void ErrorOccured() = 0;
		};
	}
}
