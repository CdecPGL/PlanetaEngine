#pragma once

#include <memory>

namespace plnt {
	namespace private_ {
		class Screen;
	}

	/*! �����_�����O�}�l�[�W���̌��J�C���^�[�t�F�C�X�B�`����Ǘ�����B*/
	class IRenderingManager {
	public:
		virtual ~IRenderingManager() = 0 { };
		/*! ���C���X�N���[�����擾����*/
		virtual std::shared_ptr<private_::Screen> GetMainScreen() = 0;
		/*! GUI�N���[�����擾����*/
		virtual std::shared_ptr<private_::Screen> GetGUIScreen() = 0;
	};
}
