#include "InitEnd.h"
#include "MemoryManager.h"

namespace PlanetaEngine{
	namespace Core{
		namespace InitEnd{
			
			bool Init()
			{
				//MemoryManager::CreateInstance();
				//MemoryManager& memm = MemoryManager::GetInstance();
				////�b��I��1MB���m�ۂ��Ă���
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