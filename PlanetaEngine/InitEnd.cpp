#include "InitEnd.h"
#include "MemoryManager.h"

namespace PlanetaEngine{
	namespace Core{
		namespace InitEnd{
			
			bool Init()
			{
				//MemoryManager::CreateInstance();
				//MemoryManager& memm = MemoryManager::GetInstance();
				////Žb’è“I‚É1MB‚¸‚ÂŠm•Û‚µ‚Ä‚¨‚­
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