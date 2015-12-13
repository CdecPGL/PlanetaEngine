#pragma once

#include <string>
#include <list>
#include <fstream>
#include "PointerSingletonTemplate.h"
#include "StringUtility.h"

namespace planeta_engine {
	namespace debug {
		//�V�X�e�����O�Ǘ�(�X�^�e�B�b�N�V���O���g��)
		class SystemLog : public utility::PointerSingletonTemplate<SystemLog>
		{
			friend utility::PointerSingletonTemplate<SystemLog>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			enum class LogLevel { Message, Warning, Error };
			/*���x�����w�肵�ă��O���o��(���x���A�������A�ڍ�(�����w�肷�邱�ƂŘA�������B))*/
			template<typename... Details>
			void Log(LogLevel level, const std::string& place, Details&&... details) {
				_Log(level, utility::ConvertAndConnectToString(std::forward<Details>(details)...), place);
			}
			/*�V���v���ȃ��O���o��*/
			template<typename... Details>
			void SimpleLog(Details&&... details) {
				_OutPutToOutStream(utility::ConvertAndConnectToString(std::forward<Details>(details)...));
			}
			/*���b�Z�[�W(�ڍׁA������)*/
			void LogMessage(const std::string& detail, const std::string& place) { _Log(LogLevel::Message, detail, place); }
			/*�x��(�ڍׁA������)*/
			void LogWarning(const std::string& detail, const std::string& place) { _Log(LogLevel::Warning, detail, place); }
			/*�G���[(�ڍׁA������)*/
			void LogError(const std::string& detail, const std::string& place) { _Log(LogLevel::Error, detail, place); }

			/*���O�o�̓X�g���[����ǉ�(�C���X�^���X�̐����A�j���͊O���ōs��)*/
			void AddLogOutStream(std::ostream& ostrm);
			/*���O�o�̓X�g���[�������ׂď���*/
			void ResetLogOutStream() { _output_streams.clear(); }
			/*���O�������w�肳�ꂽ�t�@�C���ɏo��(�G���W���̃t�@�C���V�X�e���͎g�킸�A�J�����g�f�B���N�g���Ɏw�肳�ꂽ���O�Œ��ڏo�͂���)*/
			bool DumpLogHistory(const std::string& file_name);
			/*���O�����ő�T�C�Y��ݒ�(0�Ŗ���)*/
			void SetLogHistoryMaxSize(size_t size);
		private:
			SystemLog();
			~SystemLog();
			std::list<std::ostream*> _output_streams;
			size_t _log_history_max_size; //���O�����ő�T�C�Y(0�Ŗ���)
			std::list<std::string> _log_history; //���O����

			void _Log(LogLevel level, const std::string& detail, const std::string& place);
			void _OutPutToOutStream(const std::string& str);
			void _AddHistory(const std::string& str);
		};
		using LogLevel = SystemLog::LogLevel;
	}
}
