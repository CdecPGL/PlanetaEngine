#pragma once

#include <string>
#include <list>
#include <fstream>
#include <memory>
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
			enum class LogLevel { Message, Warning, Error, Fatal };
			/*�R���\�[���ւ̏o�͂�L����*/
			bool ValidateConsoleOutPut();
			/*���x�����w�肵�ă��O���o��(���x���A�������A�ڍ�(�����w�肷�邱�ƂŘA�������B))*/
			template<typename... Details>
			void Log(LogLevel level, const std::string& place, Details&&... details) {
				_Log(level, utility::ConvertAndConnectToString(std::forward<Details>(details)...), place);
			}
			/*�V���v���ȃ��O���o��*/
			template<typename... Details>
			void SimpleLog(Details&&... details) {
				_OutPut(LogLevel::Message, utility::ConvertAndConnectToString(std::forward<Details>(details)...));
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
			void ResetLogOutStream();
			/*���O�������w�肳�ꂽ�t�@�C���ɏo��(�G���W���̃t�@�C���V�X�e���͎g�킸�A�J�����g�f�B���N�g���Ɏw�肳�ꂽ���O�Œ��ڏo�͂���)*/
			bool DumpLogHistory(const std::string& file_name);
			/*���O�����ő�T�C�Y��ݒ�(0�Ŗ���)*/
			void SetLogHistoryMaxSize(size_t size);
			enum class DumpLevel { All, Message, Warning, Error };
			/*�o�̓��x����ݒ�*/
			void SetDumpLevel();
		private:
			SystemLog();
			~SystemLog();
			class Impl_;
			std::unique_ptr<Impl_> impl_;

			void _Log(LogLevel level, const std::string& detail, const std::string& place);
			void _OutPut(LogLevel level, const std::string& str);
		};
		using LogLevel = SystemLog::LogLevel;
	}
}
