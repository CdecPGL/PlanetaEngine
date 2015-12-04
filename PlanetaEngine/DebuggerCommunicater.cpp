#include "DebuggerCommunicater.h"
#include "SystemLog.h"

#include <windows.h>
#include <thread>
#include <array>
#include <cstdio>

namespace planeta_engine {
	namespace debug {

		class DebuggerCommunicater::Impl_ {
		public:
			void CommunicationProc();
			template<typename T>
			bool SendData(const T& data) {
				DWORD result;
				*reinterpret_cast<T*>(buffer.data()) = data;
				WriteFile(hPipe, buffer.data(), sizeof(T), &result, NULL);
				return true;
			}
			template<typename T>
			T ReceiveData() {
				DWORD result;
				ReadFile(hPipe, buffer.data(), sizeof(T), &result, NULL);
				return *reinterpret_cast<T*>(buffer.data());
			}

			std::thread com_thd;
			HANDLE hPipe;
			std::array<TCHAR,256> buffer;
		};

		DebuggerCommunicater::DebuggerCommunicater():impl_(std::make_unique<Impl_>())
		{

		}

		DebuggerCommunicater::~DebuggerCommunicater()
		{

		}

		bool DebuggerCommunicater::Initialize()
		{
			impl_->hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\PlanetaEngineDebugPipe"), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, 1, sizeof(TCHAR)*impl_->buffer.size(), sizeof(TCHAR)*impl_->buffer.size(), 1000, NULL);
			if (impl_->hPipe == INVALID_HANDLE_VALUE) {
				MessageBox(NULL, TEXT("�p�C�v�̍쐬�Ɏ��s���܂����B"), NULL, MB_ICONWARNING);
				return false;
			}
			impl_->com_thd = std::move(std::thread([this]{impl_->CommunicationProc(); }));
			return true;
		}

		void DebuggerCommunicater::Finalize()
		{
			if (impl_->com_thd.joinable()) { 
//				impl_->com_thd.join(); //�N���C�A���g�Ɛڑ����Ă��Ȃ��ƃX���b�h���I���ł��Ȃ�
				impl_->com_thd.detach(); 
			}
			CloseHandle(impl_->hPipe);
		}

		void DebuggerCommunicater::Impl_::CommunicationProc() {
			ConnectNamedPipe(hPipe, nullptr); //�f�o�b�K�[�Ɛڑ�
			//������
			//�����̃v���Z�XID�𑗐M
			SendData(GetCurrentProcessId());
			//�������Q�ƃe�X�g
			long write_test_data = (long)std::rand();
			long write_test_target = 0;
			long read_test_data = (long)std::rand();
			//�ǂݎ��m�F
			SendData(&read_test_data); //�ǂݍ��ݑΏۂ̃������A�h���X���M
			long res = ReceiveData<long>(); //�ǂݍ��񂾃������A�h���X��M
			if(res==read_test_data){} //�������m�F
			//�������݊m�F
			SendData(&write_test_target); //���������Ώۂ̃������A�h���X���M
			SendData(write_test_data); //���������f�[�^���M
			ReceiveData<long>(); //�������������ҋ@
			if(write_test_target == write_test_data){} //�������m�F
			//�f�o�b�O�f�[�^���X�g�𑗐M



			//�v����ҋ@

			//�v���ɉ����ăf�[�^���M

		}

	}
}