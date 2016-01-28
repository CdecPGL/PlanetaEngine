#include "DebuggerCommunicater.h"
#include "SystemLog.h"

#include <windows.h>
#include <vector>

namespace {
	constexpr char* MapFileName("PlanetaEngineDebugCommunicater");
	constexpr int MapFileSize(2560);
	constexpr int DebugInfoMaxLength(256);
	constexpr int DebugInfoCount(10);
}

namespace planeta_engine {
	namespace debug {
		//////////////////////////////////////////////////////////////////////////
		//Impl
		//////////////////////////////////////////////////////////////////////////


		class DebuggerCommunicater::Impl_ {
		private:
			HANDLE mem_mapd_file_handle_;
			LPVOID map_address_;
			std::vector<std::string> debug_infos_;
		public:
			
			//通信用メモリマップドファイルを設定する
			bool SetUpMemoryMappedFileForCommunicate() {
				//メモリマップドファイルを作成　サイズ:1024バイト　名前:inaba
				//ページファイルを利用
				HANDLE hFile = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, MapFileSize, "PlanetaEngineDebugCommunicater");
				mem_mapd_file_handle_ = hFile;
				if (hFile == NULL) {
					//エラー
					return false;
				}
				//アドレスを取得
				LPVOID hMap = MapViewOfFile(hFile, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
				map_address_ = hMap;
				if (hMap == NULL) {
					return false;
				}	
				return true;
			}

			bool FinishMemoryMappedFileForCommunicate() {
				UnmapViewOfFile(map_address_);
				CloseHandle(mem_mapd_file_handle_);//メモリマップドファイルをクローズ
			}

			void PushdebugInformation(const std::string& info) {
				debug_infos_.push_back(info);
			}

			void DumpDebugInfoToMMF() {
				for (int i = 0; i < (signed)debug_infos_.size();++i) {
					strcpy_s((char*)mem_mapd_file_handle_ + i*DebugInfoMaxLength, DebugInfoMaxLength, debug_infos_[i].c_str());
				}
				for (int i = debug_infos_.size(); i < DebugInfoCount; ++i) {
					memset((char*)mem_mapd_file_handle_ + i*DebugInfoMaxLength, 0, DebugInfoMaxLength);
				}
				debug_infos_.clear();
			}
		};

		//////////////////////////////////////////////////////////////////////////
		//DebugerCommunicater
		//////////////////////////////////////////////////////////////////////////

		DebuggerCommunicater::DebuggerCommunicater():impl_(std::make_unique<Impl_>())
		{

		}

		DebuggerCommunicater::~DebuggerCommunicater()
		{

		}

		bool DebuggerCommunicater::Initialize()
		{
			
			return true;
		}

		void DebuggerCommunicater::Finalize()
		{
			
		}

	}
}