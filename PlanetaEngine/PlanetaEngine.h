#pragma once

//PlanetaEngine���g�����߂̊�{�I�ȃw�b�_�t�@�C���B
//main�t�@�C���ŃC���N���[�h���A��{�I�ɂق��̃t�@�C���ł̓C���N���[�h���Ȃ�����

//���C�u�����t�@�C���̎w��
#ifdef _DEBUG
#pragma comment(lib,"planetaengine_mtd.lib")
#else
#pragma comment(lib,"planetaengine_mt.lib")
#endif

#include "PlanetaEngineImpl.h"

//Reflection�V�X�e���ɓo�^���邽�߂̃w�b�_�t�@�C���W�J
//PlanetaEngine���C�u�������g�p����v���W�F�N�g���ň�x���APE_REFLECTABLE_CLASS�}�N������g���K�[�N���X����`����Ă��Ȃ��A�܂�w�b�_�t�@�C�����C���N���[�h���C���X�g�[������Ă��Ȃ��ꍇ�A�g���K�[�N���X���Ăяo����Ȃ��B
//���������邽�߂ɁA�v���W�F�N�g���ōŒ���̓C���N���[�h�����A���C���N���[�h�񐔂����ɋ߂����̃w�b�_�t�@�C���ɋL�q����B
#include "BuiltInGameObjectComponents.h"
#include "BuiltInTasks.h"
#include "BuiltInResources.h"
