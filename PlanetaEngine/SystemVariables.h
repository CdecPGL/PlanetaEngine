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
			namespace file_system {
				extern const std::string EngineConfigFileName; //�G���W���ݒ�t�@�C����
				extern const std::string ProgramDefinitionFileName; //�v���O�����p��`�t�@�C��
				extern const std::string UserConfigFileName; //���[�U�[�ݒ�t�@�C����
				extern const std::string KeyConfigFileName; //�L�[�ݒ�t�@�C����
				extern const std::string LogOutPutFileName; //���O�o�̓t�@�C����
				extern const std::string ResourceListFileName; //���\�[�X���X�g�t�@�C����
				extern const std::string ResourceDataArchiveFileName; //���\�[�X�f�[�^�A�[�J�C�u�t�@�C����
				extern const std::string DevResourceDataFolderName; //resource�J���p�f�[�^�t�H���_��(�J�����[�h���ɗL��)

				extern const std::string SystemDataDirectory; //�V�X�e���f�[�^�̃f�B���N�g��
				extern const std::string LogDirectory; //���O�̃f�B���N�g��
				extern const std::string SaveDataDirectory; //�Z�[�u�f�[�^�̃f�B���N�g��
				extern const std::string ResourceDataDirectory; //���\�[�X�f�[�^�f�B���N�g��
				extern const std::string ConfigFileDirectory; //�ݒ�t�@�C���f�B���N�g��

				extern const std::string ResourceFileAccessorID; //���\�[�X�p�t�@�C��Accessor��ID
				extern const std::string SaveDataFileAccessorID; //SameData�p�t�@�C���A�N�Z�T��ID
				extern const std::string SystemFileAccessorID; //System�p�t�@�C��Accessor��ID
				extern const std::string ConfigFileAccessorID; //Config�p�t�@�C���A�N�Z�T��ID

				extern const unsigned int ResourceDataArchiveDecryptionKey; //���\�[�X�f�[�^�A�[�J�C�u�̕������L�[
			}
			namespace resource {
				extern const std::string BuiltinGraphResourceTypeName; //�W���̉摜���\�[�X�^�C�v��
				extern const std::string BuiltinSoundResourceTypeName; //�W���̌��ʉ����\�[�X�^�C�v��
				extern const std::string BuiltinMusicResourceTypeName; //�W���̉��y���\�[�X�^�C�v��
				extern const std::string BuiltinCSVResourceTypeName; //�W����CSV�`���t�@�C�����\�[�X�^�C�v��
				extern const std::string BuiltinTextResourceTypeName; //�W���̃e�L�X�g�t�@�C�����\�[�X�^�C�v��
				extern const std::string BuiltinXMLResourceTypeName; //�W����XML�t�@�C�����\�[�X�^�C�v��
				extern const std::string BuiltinFontDefinitionResourceTypeName; //�W���̃t�H���g��`�t�@�C�����\�[�X�^�C�v��
			}
			namespace prefixes {
				extern const std::string Scene;
				extern const std::string GameObject;
				extern const std::string Task;
			}
			/*�f�o�b�O*/
			extern const bool DevelopmentMode; //�J�����[�h�t���O
		}
	}
}