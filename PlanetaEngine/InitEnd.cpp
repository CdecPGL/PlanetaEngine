#include "InitEnd.h"
#include "MemoryManager.h"

namespace PlanetaEngine{
	namespace Core{
		namespace InitEnd{
			
			bool Init()
			{
				//MemoryManager::CreateInstance();
				//MemoryManager& memm = MemoryManager::GetInstance();
				////暫定的に1MBずつ確保しておく
				//if (memm.Init(1000000, 1000000, 1000000, 1000000) == false){ return false; }
				return true;
			}

			bool End()
			{
				/*MemoryManager::DestroyInstance();
				MemoryManager& memm = MemoryManager::GetInstance();
				memm.End();*/
				return true;
			}

		}
	}
}