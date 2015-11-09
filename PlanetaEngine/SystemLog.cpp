#include "SystemLog.h"
#include "SystemCounter.h"
#include "SystemVariables.h"
#include <sstream>
#include <iostream>
#include "PEDateTime.h"

/*���O�̏���
[hh:mm:ss,frame]level:detail
*/

namespace {
	const std::string kMessageHeader("���b�Z�[�W");
	const std::string kWarningHeader("�x��");
	const std::string kErrorHeader("�G���[");
	const size_t kDefaultLogHistoryMaxSize(0);
}

namespace planeta_engine {
	namespace debug {

		bool SystemLog::Initialize()
		{
			_OutPutToOutStream(std::string("PlanetaEngine V") + core::system_variables::system_information::VersionString);
			_OutPutToOutStream(std::string("�N������:") + utility::DateTime::GetCurrentDateTime().ToString());
			if (core::system_variables::DevelopmentMode)_OutPutToOutStream("�J�����[�h���L���ł��B");
			Log(LogLevel::Message, __FUNCTION__, "����������܂����B���O�o�̓X�g���[����", _output_streams.size(), "�ł��B");
			return true;
		}

		bool SystemLog::Finalize()
		{
			ResetLogOutStream();
			_log_history.clear();
			_log_history_max_size = kDefaultLogHistoryMaxSize;
			LogMessage("�I�����������s����܂����B", __FUNCTION__);
			return true;
		}

		bool SystemLog::DumpLogHistory(const std::string& file_name)
		{
			std::ofstream ofs(file_name);
			if (ofs.bad()) { 
				Log(LogLevel::Warning, __FUNCTION__, "���O�������t�@�C���ɏo�͂ł��܂���ł����B�t�@�C��(", file_name, ")���J���܂���");
				return false; 
			}
			for (const auto& str : _log_history) {
				ofs << str << std::endl;
			}
			Log(LogLevel::Message, __FUNCTION__, "���O�������t�@�C��(", file_name, ")�ɏo�͂��܂����B");
			return true;
		}

		void SystemLog::_Log(LogLevel level, const std::string& detail, const std::string& place)
		{
			using namespace std;
			string header;
			switch (level)
			{
			case LogLevel::Message:
				header = kMessageHeader;
				break;
			case LogLevel::Warning:
				header = kWarningHeader;
				break;
			case LogLevel::Error:
				header = kErrorHeader;
				break;
			default:
				break;
			}
			//�o�͕�����𐶐�
			stringstream sstrm;
			//���O�̃V�X�e������
			sstrm << '[' << SystemCounter::instance().GetCurrentTimeCount().ToString() << ',' << SystemCounter::instance().GetCurrentFrameCount() << ']';
			//���O�̃��x��
			sstrm << header << ':';
			//�ڍ�
			sstrm << detail;
			//�ꏊ
			sstrm << '@' << place;
			//�o��
			_OutPutToOutStream(sstrm.str());
			//���O�����ɒǉ�
			_AddHistory(sstrm.str());
		}

		void SystemLog::_OutPutToOutStream(const std::string& str)
		{
			for (auto& ostrm : _output_streams)
			{
				*ostrm << str << std::endl;
			}
		}

		SystemLog::SystemLog():_log_history_max_size(kDefaultLogHistoryMaxSize)
		{

		}

		SystemLog::~SystemLog()
		{

		}

		void SystemLog::_AddHistory(const std::string& str)
		{
			_log_history.push_back(str);
			if (_log_history_max_size == 0) { return; }
			//���O�����̍ő�l�𒴂��Ă�����퓬�̃��O���폜����
			if (_log_history.size() > _log_history_max_size) { _log_history.pop_front(); }
		}

		void SystemLog::AddLogOutStream(std::ostream& ostrm)
		{
			_output_streams.push_back(&ostrm);
//			LogMessage("���O�o�̓X�g���[����ǉ����܂����B", "SystemLog::AddLogOutStream");
		}

		void SystemLog::SetLogHistoryMaxSize(size_t size)
		{
			_log_history_max_size = size;
			Log(LogLevel::Message, __FUNCTION__, "���O�����̍ő�T�C�Y��", size, "�ɕύX���܂����B");
		}

	}
}