#pragma once

#include <string>

namespace planeta_engine{
	namespace core{
		namespace system_variables{
			namespace engine_information {
				extern const std::string VersionString; //�o�[�W����������
				extern const int MajorVersionNumber; //���W���[�o�[�W�����ԍ�
				extern const int MinorVersionNumber; //�}�C�i�[�o�[�W�����ԍ�
				extern const int SubVesionNumber; //�T�u�o�[�W�����ԍ�
				extern const std::string BuildDate; //�r���h�������t
			}
			namespace system_file {
				extern const std::string ConfigFileName; //�ݒ�t�@�C����
			}
			/*�f�o�b�O*/
			extern const bool DevelopmentMode; //�J�����[�h�t���O
			/*�f�B���N�g��*/
			extern const std::string SystemDataDirectory; //�V�X�e���f�[�^�̃f�B���N�g��
			extern const std::string LogDirectory; //���O�̃f�B���N�g��
			extern const std::string SaveDataDirectory; //�Z�[�u�f�[�^�̃f�B���N�g��
			/*�V�X�e�����O*/
			extern const std::string LogOutPutFileName; //���O�o�͐�t�@�C����
			/*�t�@�C���V�X�e��*/
			extern const std::string ResourceDataArchiveFilePath; //���\�[�X�f�[�^�A�[�J�C�u�t�@�C���̃p�X
			extern const unsigned int ResourceDataArchiveDecryptionKey; //���\�[�X�f�[�^�A�[�J�C�u�̕������L�[
			extern const std::string DevResourceDataFolderPath; //resource�J���p�f�[�^�t�H���_�̃p�X(�J�����[�h���ɗL��)
			extern const std::string ResourceFileAccessorID; //���\�[�X�p�t�@�C��Accessor��ID
			extern const std::string GameDataFileAccessorID; //GameData�p�t�@�C���A�N�Z�T��ID
			/*���\�[�X*/
			extern const std::string ResourceListFileName; //���\�[�X���X�g�t�@�C���^�C�v��
			extern const std::string BuiltinGraphResourceTypeName; //�W���̉摜���\�[�X�^�C�v��
			extern const std::string BuiltinSoundResourceTypeName; //�W���̌��ʉ����\�[�X�^�C�v��
			extern const std::string BuiltinMusicResourceTypeName; //�W���̉��y���\�[�X�^�C�v��
			extern const std::string BuiltinCSVResourceTypeName; //�W����CSV�`���t�@�C�����\�[�X�^�C�v��
			extern const std::string BuiltinTextResourceTypeName; //�W���̃e�L�X�g�t�@�C�����\�[�X�^�C�v��
			extern const std::string BuiltinXMLResourceTypeName; //�W����XML�t�@�C�����\�[�X�^�C�v��
			extern const std::string BuiltinFontDefinitionResourceTypeName; //�W���̃t�H���g��`�t�@�C�����\�[�X�^�C�v��
			/*����v���Z�X�D��x*/
			namespace process{
				extern const int CollisionDetectProcessPriority; //�Փ˔���v���Z�X�D��x
				extern const int GameObjectDrawProcessPriority; //�Q�[���I�u�W�F�N�g�`��v���Z�X�D��x
				extern const int GameObjectUpdatetProcessPriority; //�Q�[���I�u�W�F�N�g�X�V�v���Z�X�D��x
				extern const int UIDrawProcessPriority; //UI�`��v���Z�X�D��x
				extern const int UIUpdateProcessPriority; //UI�X�V�v���Z�X�D��x
				extern const int TransformApplyVelocityProcessPriority; //�g�����X�t�H�[�����x�K�p�v���Z�X�D��x
				//extern const int TransformConvertLocalToGlobalProcessPriority; //�g�����X�t�H�[�����[�J���O���[�o���ϊ��v���Z�X�D��x
				//extern const int TransformConvertGlobalToLocalProcessPriority; //�g�����X�t�H�[���O���[�o�����[�J���ϊ��v���Z�X�D��x
				extern const std::string CollisionDetectProcessName; //�Փ˔���v���Z�X��
				extern const std::string GameObjectDrawProcessName; //�Q�[���I�u�W�F�N�g�`��v���Z�X��
				extern const std::string GameObjectUpdatetProcessName; //�Q�[���I�u�W�F�N�g�X�V�v���Z�X��
				extern const std::string UIDrawProcessName; //UI�`��v���Z�X��
				extern const std::string UIUpdateProcessName; //UI�X�V�v���Z�X��
				extern const std::string TransformApplyVelocityProcessName; //�g�����X�t�H�[�����x�K�p�v���Z�X��
				//extern const std::string TransformConvertLocalToGlobalProcessName; //�g�����X�t�H�[�����[�J���O���[�o���ϊ��v���Z�X��
				//extern const std::string TransformConvertGlobalToLocalProcessName; //�g�����X�t�H�[���O���[�o�����[�J���ϊ��v���Z�X��
			}
		}
	}
}