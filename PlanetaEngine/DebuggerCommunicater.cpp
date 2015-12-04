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
				MessageBox(NULL, TEXT("パイプの作成に失敗しました。"), NULL, MB_ICONWARNING);
				return false;
			}
			impl_->com_thd = std::move(std::thread([this]{impl_->CommunicationProc(); }));
			return true;
		}

		void DebuggerCommunicater::Finalize()
		{
			if (impl_->com_thd.joinable()) { 
//				impl_->com_thd.join(); //クライアントと接続していないとスレッドを終了できない
				impl_->com_thd.detach(); 
			}
			CloseHandle(impl_->hPipe);
		}

		void DebuggerCommunicater::Impl_::CommunicationProc() {
			ConnectNamedPipe(hPipe, nullptr); //デバッガーと接続
			//初期化
			//自分のプロセスIDを送信
			SendData(GetCurrentProcessId());
			//メモリ参照テスト
			long write_test_data = (long)std::rand();
			long write_test_target = 0;
			long read_test_data = (long)std::rand();
			//読み取り確認
			SendData(&read_test_data); //読み込み対象のメモリアドレス送信
			long res = ReceiveData<long>(); //読み込んだメモリアドレス受信
			if(res==read_test_data){} //整合性確認
			//書き込み確認
			SendData(&write_test_target); //書き換え対象のメモリアドレス送信
			SendData(write_test_data); //書き換えデータ送信
			ReceiveData<long>(); //書き換え完了待機
			if(write_test_target == write_test_data){} //整合性確認
			//デバッグデータリストを送信



			//要求を待機

			//要求に応じてデータ送信

		}

	}
}