#pragma once
#include <functional>

namespace planeta_engine{
	namespace core{
		namespace init_end{
			/*�ȉ��V�X�e���֐�*/
			bool InitDxLibrary(); //DX���C�u�����̏�����
			bool EndDxLibrary(); //DX���C�u�����̏I������

			bool LoadConfigData(); //�ݒ�t�@�C����ǂݍ���

			bool SetUpSingletonManagers(); //�V���O���g���}�l�[�W���̃Z�b�g�A�b�v

			bool InitializeSingletonManagers(); //�V���O���g���}�l�[�W���̏�����
			bool FinalizeSingletonManagers(); //�V���O���g���}�l�[�W���̏I������

			//�e��ݒ�֐�(�������͍s��Ȃ�)
			bool SetUpSystemLog(); //�V�X�e�����O�̐ݒ�
			bool SetUpFileSystem(); //�t�@�C���V�X�e���̐ݒ�B���\�[�X�}�l�[�W���̐ݒ���s��
			bool SetUpResourceManager(); //���\�[�X�}�l�[�W���̐ݒ�
			//�e��I�������֐�(�I���������s��)
			bool FinalizeSystemLog(); //�V�X�e�����O�̏I������

			bool InitializeDebugSystem(); //�f�o�b�O�V�X�e���̏�����
			bool FinalizeDebugStstem(); //�f�o�b�O�V�X�e���̏I������

			void CreateSystemDirectory(); //�K�v�Ȃ�V�X�e���p�̃f�B���N�g�����쐬����
		}
	}
}