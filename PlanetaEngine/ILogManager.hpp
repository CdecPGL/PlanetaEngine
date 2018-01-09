#pragma once

#include <string>
#include "StringUtility.h"

namespace planeta {
	enum class LogLevel { Message, Warning, Error, Fatal };
	/*! ���O�}�l�[�W�����J�C���^�[�t�F�C�X*/
	class ILogManager {
	public:
		virtual ~ILogManager() = 0 {};
		/*! ���x�����w�肵�ă��O���o��(���x���A�������A�ڍ�(�����w�肷�邱�ƂŘA�������B))*/
		template<typename... Details>
		void Log(LogLevel level, const std::string& place, Details&&... details) {
			LogProc(level, util::ConvertAndConnectToString(std::forward<Details>(details)...), place);
		}
		/*! �V���v���ȃ��O���o��*/
		template<typename... Details>
		void SimpleLog(Details&&... details) {
			SimpleLogProc(util::ConvertAndConnectToString(std::forward<Details>(details)...));
		}
	private:
		virtual void LogProc(LogLevel level, const std::string& detail, const std::string& place) = 0;
		virtual void SimpleLogProc(const std::string& detail) = 0;
	};
}
