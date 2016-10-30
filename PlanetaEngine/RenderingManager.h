#pragma once

#include "IRenderingManager.h"
#include "SubSystemManager.h"

namespace planeta {
	namespace private_ {
		class Screen;
		/*�����_�����O�}�l�[�W��*/
		class RenderingManager : public IRenderingManager, public SubSystemManager {
		public:
			virtual ~RenderingManager()override = default;
			/*����������*/
			virtual bool Initialize() = 0;
			/*�I���������s��*/
			virtual void Finalize() = 0;
			/*�X�V����*/
			virtual bool Update() = 0;
			/*�f�o�b�O�`��p�X�N���[�����擾����*/
			virtual std::shared_ptr<private_::Screen> GetDebugDrawScreen() = 0;
			/*�f�o�b�O���`��p�X�N���[�����擾����*/
			virtual std::shared_ptr<private_::Screen> GetDebugInformationScreen() = 0;
		};
	}
}