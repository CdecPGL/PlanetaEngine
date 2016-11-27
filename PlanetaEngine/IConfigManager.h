#pragma once

#include <string>
#include <array>
#include "Vector2D.h"
#include "CollisionGroupMatrix.h"

namespace planeta {
	class IConfigManager {
	public:
		virtual ~IConfigManager() = 0 {};
		/*! �Q�[���^�C�g�����擾����*/
		virtual std::string game_title()const = 0;
		/*! �Q�[���̃o�[�W������\����������擾����*/
		virtual std::string game_version_string()const = 0;
		/*! �Q�[���̃o�[�W������\�����l�z����擾����[Major, Minor, Sub]*/
		virtual std::array<int, 3> game_version_numbers()const = 0;
		/*! �E�C���h�E���[�h���ǂ����̃t���O���擾����*/
		virtual bool is_window_mode()const = 0;
		/*! �`��T�C�Y���擾����B�v���O�������ł̉�ʂ̍L����\��*/
		virtual Vector2Di draw_size()const = 0;
		/*! �E�C���h�E�T�C�Y���擾����B�\��������ʂ̃T�C�Y��\��*/
		virtual Vector2Di window_size()const = 0;
		/*! �Փ˃O���[�v�}�g���N�X���擾����*/
		virtual const CollisionGroupMatrix& collision_group_matrix()const = 0;
		/*! Cursor��\�����邩�ǂ������擾����*/
		virtual bool is_cursor_visible()const = 0;
	};
}
