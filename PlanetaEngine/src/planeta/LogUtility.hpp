#pragma once

#include "Game.hpp"
#include "ILogManager.hpp"

//メッセージを出力する
#define PE_LOG_MESSAGE(...)\
::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Message,__FUNCTION__,__VA_ARGS__)
//警告を出力する
#define PE_LOG_WARNING(...)\
::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Warning,__FUNCTION__,__VA_ARGS__)
//エラーを出力する
#define PE_LOG_ERROR(...)\
::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Error,__FUNCTION__,__VA_ARGS__)
//致命的エラーを出力する(デバッグモードの時は、プログラムを停止させる)
#define PE_LOG_FATAL(...)\
::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Fatal,__FUNCTION__,__VA_ARGS__)
//条件チェック(引数がfalseなら致命的エラーを発生させる)
#define PE_VERIFY(condition)\
if((condition)==false){::plnt::Game::instance().log_manager()->Log(::plnt::LogLevel::Fatal,__FUNCTION__,"expression : ",#condition);}
