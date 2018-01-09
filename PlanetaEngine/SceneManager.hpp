#pragma once

#include <memory>

#include "ISceneManager.h"
#include "SubSystemManager.h"

namespace planeta {
	class CollisionGroupMatrix;
	namespace private_ {
		class ResourceManager;
		/*�V�[���̏�(�p���A�I���A�G���[����)*/
		enum class SceneStatus_ { Continue, Quit, Error };
		/*�V�[���}�l�[�W��*/
		class SceneManager : public ISceneManager, public SubSystemManager {
		public:
			virtual ~SceneManager()override = default;
			/*�}�l�[�W���̏�����*/
			virtual bool Initialize() = 0;
			/*�}�l�[�W���̏I������*/
			virtual bool Finalize() = 0;
			/*�V�[������*/
			virtual SceneStatus_ Process_() = 0;
			/*���\�[�X�}�l�[�W�����Z�b�g(�������O)*/
			virtual void SetResouceManager(const std::shared_ptr<ResourceManager>& mgr) = 0;
		};
	}
}