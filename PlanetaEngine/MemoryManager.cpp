#include "MemoryManager.h"
#include "MemoryPools.h"
#include "TLSFMemoryAllocator.h"

namespace planeta_engine{
	namespace core{

		MemoryManager* MemoryManager::_instance = nullptr;
		
		bool MemoryManager::Init(size_t eg_pool_size, size_t go_pool_size, size_t pr_pool_size, size_t ui_pool_size)
		{
			using namespace MemoryPools;
			engine_memory_pool = new Allocators::TLSFMemoryAllocator;
			game_object_memory_pool = new Allocators::TLSFMemoryAllocator;
			process_memory_pool = new Allocators::TLSFMemoryAllocator;
			ui_memory_pool = new Allocators::TLSFMemoryAllocator;
			engine_memory_pool->ReserveMemory(eg_pool_size);
			game_object_memory_pool->ReserveMemory(go_pool_size);
			process_memory_pool->ReserveMemory(pr_pool_size);
			ui_memory_pool->ReserveMemory(ui_pool_size);
			return true;
		}

		bool MemoryManager::End()
		{
			using namespace MemoryPools;
			if (engine_memory_pool){ delete engine_memory_pool; }
			if (game_object_memory_pool){ delete game_object_memory_pool; }
			if (process_memory_pool){ delete process_memory_pool; }
			if (ui_memory_pool){ delete ui_memory_pool; }
			return true;
		}

		bool MemoryManager::Process()
		{
			return true;
		}

		MemoryManager::MemoryManager()
		{

		}

		MemoryManager::~MemoryManager()
		{

		}

	}
}