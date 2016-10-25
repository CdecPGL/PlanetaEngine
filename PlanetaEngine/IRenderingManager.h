#pragma once

#include <memory>

namespace planeta {
	namespace private_ {
		class Screen;
	}
	/*! �����_�����O�}�l�[�W���̌��J�C���^�[�t�F�C�X�B�`����Ǘ�����B*/
	class IRenderingManager {
	public:
		virtual ~IRenderingManager() = 0 {};
		/*! �V�����X�N���[�����쐬����(����͈�̃X�N���[���ɂ̂ݑΉ�)*/
		virtual std::shared_ptr<private_::Screen> CreateScreen() = 0;
		/*! �X�N���[����j������*/
		virtual bool DisposeScreen(const std::shared_ptr<private_::Screen>& screen) = 0;
	};
}