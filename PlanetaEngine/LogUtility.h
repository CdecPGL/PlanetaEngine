#pragma once

#include "Game.h"
#include "ILogManager.h"

//メッセージを出力する
#define PE_LOG_MESSAGE(...)\
::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Message,__FUNCTION__,__VA_ARGS__)
//警告を出力する
#define PE_LOG_WARNING(...)\
::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Warning,__FUNCTION__,__VA_ARGS__)
//エラーを出力する
#define PE_LOG_ERROR(...)\
::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Error,__FUNCTION__,__VA_ARGS__)
//致命的エラーを出力する(デバッグモードの時は、プログラムを停止させる)
#define PE_LOG_FATAL(...)\
::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Fatal,__FUNCTION__,__VA_ARGS__)
//条件チェック(引数がfalseなら致命的エラーを発生させる)
#define PE_VERIFY(condition)\
if((condition)==false){::planeta::Game::instance().log_manager()->Log(::planeta::LogLevel::Fatal,__FUNCTION__,"expression : ",#condition);}
