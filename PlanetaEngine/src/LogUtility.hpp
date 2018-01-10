#pragma once

#include "Game.hpp"
#include "ILogManager.hpp"

//���b�Z�[�W���o�͂���
#define PE_LOG_MESSAGE(...)\
::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Message,__FUNCTION__,__VA_ARGS__)
//�x�����o�͂���
#define PE_LOG_WARNING(...)\
::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Warning,__FUNCTION__,__VA_ARGS__)
//�G���[���o�͂���
#define PE_LOG_ERROR(...)\
::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Error,__FUNCTION__,__VA_ARGS__)
//�v���I�G���[���o�͂���(�f�o�b�O���[�h�̎��́A�v���O�������~������)
#define PE_LOG_FATAL(...)\
::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Fatal,__FUNCTION__,__VA_ARGS__)
//�����`�F�b�N(������false�Ȃ�v���I�G���[�𔭐�������)
#define PE_VERIFY(condition)\
if((condition)==false){::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Fatal,__FUNCTION__,"expression : ",#condition);}
