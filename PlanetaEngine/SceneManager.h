#pragma once

#include <memory>

#include "ISceneManager.h"
#include "NonCopyable.h"

namespace planeta {
	class CollisionGroupMatrix;
	namespace private_ {
		class ResourceManager;
		/*�V�[���̏�(�p���A�I���A�G���[����)*/
		enum class SceneStatus_ { Continue, Quit, Error };
		/*�V�[���}�l�[�W��*/
		class SceneManager : public ISceneManager, private util::NonCopyable<SceneManager> {
		public:
			virtual ~SceneManager()override = default;
			/*�}�l�[�W���̏�����*/
			virtual bool Initialize() = 0;
			/*�}�l�[�W���̏I������*/
			virtual bool Finalize() = 0;
			/*�V�[������*/
			virtual SceneStatus_ Process_() = 0;
			/*�Փ˃O���[�v�}�b�v��o�^*/
			virtual void SetCollisionGroupMatrix_(std::shared_ptr<CollisionGroupMatrix>&& cg_matrix) = 0;
			/*���\�[�X�}�l�[�W�����Z�b�g(�������O)*/
			virtual void SetResouceManager(const std::shared_ptr<ResourceManager>& mgr) = 0;
		};
	}
}