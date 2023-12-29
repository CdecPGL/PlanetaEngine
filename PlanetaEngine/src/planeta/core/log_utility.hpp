#pragma once

#include "game.hpp"
#include "i_log_manager.hpp"

//メッセージを出力する
#define PE_LOG_MESSAGE(...)\
::plnt::game::instance().log_manager()->log(::plnt::log_level::message,__FUNCTION__,__VA_ARGS__)
//警告を出力する
#define PE_LOG_WARNING(...)\
::plnt::game::instance().log_manager()->log(::plnt::log_level::warning,__FUNCTION__,__VA_ARGS__)
//エラーを出力する
#define PE_LOG_ERROR(...)\
::plnt::game::instance().log_manager()->log(::plnt::log_level::error,__FUNCTION__,__VA_ARGS__)
//致命的エラーを出力する(デバッグモードの時は、プログラムを停止させる)
#define PE_LOG_FATAL(...)\
::plnt::game::instance().log_manager()->log(::plnt::log_level::fatal,__FUNCTION__,__VA_ARGS__)
//条件チェック(引数がfalseなら致命的エラーを発生させる)
#define PE_VERIFY(condition)\
if((condition)==false){::plnt::game::instance().log_manager()->log(::plnt::log_level::fatal,__FUNCTION__,"expression : ",#condition);}
