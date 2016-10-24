#pragma once

#include "Game.h"
#include "ILogManager.h"

//���b�Z�[�W���o�͂���
#define PE_LOG_MESSAGE(...)\
::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Message,__FUNCTION__,__VA_ARGS__)
//�x�����o�͂���
#define PE_LOG_WARNING(...)\
::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Warning,__FUNCTION__,__VA_ARGS__)
//�G���[���o�͂���
#define PE_LOG_ERROR(...)\
::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Error,__FUNCTION__,__VA_ARGS__)
//�v���I�G���[���o�͂���(�f�o�b�O���[�h�̎��́A�v���O�������~������)
#define PE_LOG_FATAL(...)\
::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Fatal,__FUNCTION__,__VA_ARGS__)
//�����`�F�b�N(������false�Ȃ�v���I�G���[�𔭐�������)
#define PE_VERIFY(condition)\
if((condition)==false){::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Fatal,__FUNCTION__,"expression : ",#condition);}
