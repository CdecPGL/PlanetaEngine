#pragma once

#include "GameObjectComponent.h"

namespace planeta {
	class GameObjectStandardComponent : public GameObjectComponent {
	public:
		virtual ~GameObjectStandardComponent() = default;
	private:
		/*�V�[���f�[�^�̃Z�b�g�͍s��Ȃ��B�q�N���X�ɂ̓I�[�o�[���C�h�s�ɂ��Ă����B*/
		void SetSceneData(const util::WeakPointer<core::SceneData>&)override final{ return; }
	};
}