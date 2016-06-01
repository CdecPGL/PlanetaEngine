#include "DxLib.h"

#include "SystemLog.h"
#include "SystemTimer.h"
#include "SystemVariables.h"
#include <sstream>
#include <iostream>
#include <windows.h>
#include <cassert>
#include "PEDateTime.h"
#include "WindowsUtility.h"

/*���O�̏���
[hh:mm:ss,frame]level:detail@place
*/

namespace {
	constexpr char* kMessageHeader("M");
	constexpr char* kWarningHeader("W");
	constexpr char* kErrorHeader("E");
	constexpr char* kFatalErrorHeader("F");
	constexpr size_t kDefaultLogHistoryMaxSize(100);

	void OpenConsoleWindow() {
		AllocConsole();
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONIN$", "r", stdin);
	}
	void CloseConsoleWindow() {
		FreeConsole();
	}

	std::string AddNewLineIfNeed(const std::string& str) {
		std::string out = str;
		if (str.size() > 0 && str[str.size() - 1] != '\n') {
			out += '\n';
		}
		return std::move(out);
	}
}

namespace planeta {
	namespace debug {
		//////////////////////////////////////////////////////////////////////////
		//SystemLog::Impl_
		//////////////////////////////////////////////////////////////////////////
		class SystemLog::Impl_ {
		public:
			Impl_(SystemLog& ins) :ins_(ins) {}
			~Impl_() = default;

			std::list<std::ostream*> _output_streams;
			size_t _log_history_max_size = kDefaultLogHistoryMaxSize; //���O�����ő�T�C�Y(0�Ŗ���)
			std::list<std::string> _log_history; //���O����
			bool output_console_flag_ = false;

			static std::string _GetStringFormatedByLogLevel(LogLevel level, const std::string& detail, const std::string& place) {
				using namespace std;
				string header;
				switch (level) {
				case LogLevel::Message:
					header = kMessageHeader;
					break;
				case LogLevel::Warning:
					header = kWarningHeader;
					break;
				case LogLevel::Error:
					header = kErrorHeader;
					break;
				case LogLevel::Fatal:
					header = kFatalErrorHeader;
					break;
				default:
					break;
				}
				//�o�͕�����𐶐�
				stringstream sstrm;
				//���O�̃V�X�e������
				sstrm << '[' << SystemTimer::instance().GetCurrentTimeCount().ToString() << ',' << SystemTimer::instance().GetCurrentFrameCount() << ']';
				//���O�̃��x��
				sstrm << header << ':';
				//�ڍ�
				sstrm << detail;
				//�ꏊ
				sstrm << '@' << place;
				return std::move(sstrm.str());
			}

			void _OutPutToOutStream(const std::string& str) {
				std::string ostr = AddNewLineIfNeed(str);
				for (auto& ostrm : _output_streams) {
					*ostrm << ostr;
				}
			}

			void _AddHistory(const std::string& str) {
				_log_history.push_back(str);
				if (_log_history_max_size == 0) { return; }
				//���O�����̍ő�l�𒴂��Ă�����퓬�̃��O���폜����
				if (_log_history.size() > _log_history_max_size) { _log_history.pop_front(); }
			}

			void _OutPutToConsole(const std::string& str, LogLevel level) {
				int col = 0;
				switch (level) {
				case planeta::debug::SystemLog::LogLevel::Message:
					col = windows::console::COL_GRAY;
					break;
				case planeta::debug::SystemLog::LogLevel::Warning:
					col = windows::console::COL_YELLOW;
					break;
				case planeta::debug::SystemLog::LogLevel::Error:
					col = windows::console::COL_RED;
					break;
				case planeta::debug::SystemLog::LogLevel::Fatal:
					col = windows::console::COL_VIOLET;
					break;
				default:
					assert(false);
					break;
				}
				windows::console::SetCharacterColor(col);
				std::cout << AddNewLineIfNeed(str);
			}

			void _AssertionByLevel(LogLevel level,const std::string err_str) {
				if (level == LogLevel::Fatal) {
#ifndef NDEBUG
					int ret = MessageBox(GetMainWindowHandle(), "�v���I�ȃG���[���������܂����B�u���[�N�|�C���g���쐬���܂����H", "�v���I�G���[", MB_YESNO | MB_ICONERROR | MB_APPLMODAL);
					switch (ret) {
					case IDYES:
						DebugBreak();
						break;
					case IDNO:
						return;
					default:
						assert(false);
						return;
					}
#endif
				}
			}
		private:
			SystemLog& ins_;
		};

		//////////////////////////////////////////////////////////////////////////
		//SystemLog
		//////////////////////////////////////////////////////////////////////////

		SystemLog::SystemLog() :impl_(std::make_unique<Impl_>(*this)) {}

		SystemLog::~SystemLog() = default;

		bool SystemLog::Initialize()
		{
			bool debug_mode = false;
#ifdef _DEBUG
			debug_mode = true;
#else
			debug_mode = false;
#endif
			SimpleLog(std::string("PlanetaEngine v") + core::system_variables::engine_information::VersionString, debug_mode ? " �f�o�b�O�r���h" : "");
			SimpleLog(std::string("�N������:") + util::DateTime::GetCurrentDateTime().ToString());
			if (core::system_variables::DevelopmentMode) { SimpleLog("�J�����[�h���L���ł��B"); }
			PE_LOG_MESSAGE("���O�o�͂��J�n����܂����B���O�o�̓X�g���[����", impl_->_output_streams.size(), "�ł��B");
			return true;
		}

		void SystemLog::Finalize()
		{
			PE_LOG_MESSAGE("�V�X�e�����O���I�����܂��B");
			ResetLogOutStream();
			if (impl_->output_console_flag_) {
				CloseConsoleWindow();
				impl_->output_console_flag_ = false;
			}
			impl_->_log_history.clear();
			impl_->_log_history_max_size = kDefaultLogHistoryMaxSize;
		}

		bool SystemLog::DumpLogHistory(const std::string& file_name)
		{
			std::ofstream ofs(file_name);
			if (ofs.bad()) { 
				Log(LogLevel::Warning, __FUNCTION__, "���O�������t�@�C���ɏo�͂ł��܂���ł����B�t�@�C��(", file_name, ")���J���܂���");
				return false;
			}
			for (const auto& str : impl_->_log_history) {
				ofs << str << std::endl;
			}
			Log(LogLevel::Message, __FUNCTION__, "���O�������t�@�C��(", file_name, ")�ɏo�͂��܂����B");
			return true;
		}

		void SystemLog::_OutPut(LogLevel level, const std::string& str) {
			impl_->_OutPutToOutStream(str);
			//���O�����ɒǉ�
			impl_->_AddHistory(str);
			//�R���\�[���ɏo��
			if (impl_->output_console_flag_) { impl_->_OutPutToConsole(str, level); }
			//�f�o�b�O�E�C���h�E�ɏo��
			OutputDebugString(AddNewLineIfNeed(str).c_str());
		}		

		void SystemLog::AddLogOutStream(std::ostream& ostrm)
		{
			impl_->_output_streams.push_back(&ostrm);
//			LogMessage("���O�o�̓X�g���[����ǉ����܂����B", "SystemLog::AddLogOutStream");
		}

		void SystemLog::SetLogHistoryMaxSize(size_t size)
		{
			impl_->_log_history_max_size = size;
			Log(LogLevel::Message, __FUNCTION__, "���O�����̍ő�T�C�Y��", size, "�ɕύX���܂����B");
		}

		bool SystemLog::ValidateConsoleOutPut() {
			OpenConsoleWindow();
			impl_->output_console_flag_ = true;
			SimpleLog("�R���\�[���ւ̃��O�o�͂�L�������܂����B");
			return true;
		}

		void SystemLog::_Log(LogLevel level, const std::string& detail, const std::string& place) {
			std::string str = std::move(impl_->_GetStringFormatedByLogLevel(level, detail, place));
			_OutPut(level, str);
			impl_->_AssertionByLevel(level, str);
		}

		void SystemLog::ResetLogOutStream() { impl_->_output_streams.clear(); }

	}
}